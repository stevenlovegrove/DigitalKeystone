/* This file is part of DigitalKeystone.
 * http://github.com/stevenlovegrove/DigitalKeystone
 *
 * Copyright (c) 2011 Steven Lovegrove
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use,
 * copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following
 * conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <pangolin/pangolin.h>
#include <pangolin/video.h>
#include <pangolin/cg.h>
#include <pangolin/simple_math.h>
#include <boost/thread.hpp>


using namespace pangolin;
using namespace std;

inline void vert(double* m, double a)
{
    m[0] = 1;
    m[1] = 0;
    m[2] = 0;

    m[3] = 0;
    m[4] = cos(a);
    m[5] = -sin(a);

    m[6] = 0;
    m[7] = sin(a);
    m[8] = cos(a);
}

inline void horiz(double* m, double a)
{
    m[0] = cos(a);
    m[1] = 0;
    m[2] = sin(a);

    m[3] = 0;
    m[4] = 1;
    m[5] = 0;

    m[6] = -sin(a);
    m[7] = 0;
    m[8] = cos(a);
}

void PlayVideo(const std::string uri)
{
    VideoInput video(uri);
    const unsigned w = video.Width();
    const unsigned h = video.Height();

    // Create Glut window
    const int pw = 200;
    pangolin::CreateGlutWindowAndBind("Main",w+pw,h);

    // Load Cg program
    CgLoader  cgLoader;
    CgProgram fsKeystone = cgLoader.LoadProgram("keystone.cg","fKeystone",false);
    CGparameter fsH = cgGetNamedParameter( fsKeystone.mProg, "H");
    double Hv[3*3];
    double Hh[3*3];
    double H[] = {
        1,0,0,
        0,1,0,
        0,0,1
    };

    // Create viewport for video with fixed aspect
    View& vPanal = CreatePanel("ui")
            .SetBounds(1.0,0.0,0,pw);
    View& vVideo = Display("Video")
            .SetBounds(1.0,0.0,pw,1.0)
            .SetAspect((float)w/h);

    // OpenGl Texture for video frame
    GlTexture texVideo(w,h,GL_RGBA8);

    static Var<double> x("ui.x",0,-1.0,+1.0);
    static Var<double> y("ui.y",0,-1.0,+1.0);
    static Var<double> angle_h("ui.h",0,-M_PI,M_PI);
    static Var<double> angle_v("ui.v",0,-M_PI,M_PI);

    unsigned char* rgb = new unsigned char[video.Width()*video.Height()*3];

    for(int frame=0; !pangolin::ShouldQuit(); ++frame)
    {
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        video.GrabNext(rgb,true);
        texVideo.Upload(rgb,GL_RGB,GL_UNSIGNED_BYTE);

        // Update warp params
        horiz(Hh,angle_h);
        vert(Hv,angle_v);
        pangolin::MatMul<3,3,3>(H,Hh,Hv);
        cgGLSetMatrixParameterdr(fsH,H);
        cgUpdateProgramParameters(fsKeystone.mProg);

        // Activate video viewport and render texture
        vVideo.Activate();
        cgLoader.EnableProgram(fsKeystone);
        texVideo.RenderToViewportFlipY();
        cgLoader.DisablePrograms();

        vPanal.Render();

        // Swap back buffer with front
        glutSwapBuffers();

        // Process window events via GLUT
        glutMainLoopEvent();

        // Slow video down
        boost::this_thread::sleep(boost::posix_time::milliseconds(30));
    }

    delete[] rgb;
}

int main( int argc, char* argv[] )
{
    if( argc == 2 )
    {
        PlayVideo(argv[1]);
    }else{
        cout << "DigitalKeystone allows you to adjust keystoning digitally for projectors which lack the facility." << endl << endl;
        cout << "Usage" << endl;
        cout << "\t" << argv[0] << " filename" << endl;
    }
}
