#include <iostream>
#include <cstdlib>
#include <time.h>
#include <vector>
#include <list>
#include <cmath>
#include "boost/unordered_map.hpp"
#include <stdio.h>
#include <unistd.h>
#define GetCurrentDir getcwd
#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/GlslProg.h"
#include "cinder/Ray.h"
#include "cinder/gl/Vbo.h"
#include "cinder/gl/Fbo.h"
#include "cinder/gl/Light.h"
#include "cinder/gl/Material.h"
#include "abstractca.h"
#include "Camera.h"
#include "CellularAutomata.h"
#include "Flocking.h"
#include "wolframca.h"
#include "gameoflife.h"
#include "Math.h"
#include "LSystem.h"
#include "Strange.h"
#include <qapplication.h>
#include "X11/Xlib.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <cinder/app/AppImplQtBasic.h>

using namespace ci;
using namespace gl;
using namespace ci::app;

class ArtificialLife : public AppBasic {

public:

    ArtificialLife() : wDown(false), aDown(false), sDown(false), dDown(false), qDown(false), eDown(false), mPickMode(true),
        spaceDown(false), mCamera(Vec3f(500.0f, 500.0f, 500.0f)), gameOfLife(300, 175), flock(0), mode(WOLFRAM), clear(false),
        ruleSet(50), lSystem("F+[+F-F-F]-[-F+F+F]", alife::rule_set()), lSystemIteration(0), lSystemScale(1), strange2d(false), strange3d(true) {}

    ~ArtificialLife();

    void prepareSettings(Settings *settings);
    void setup();
    void quit();
    void mouseDown(MouseEvent event);
    void mouseUp(MouseEvent event);
    void mouseMove(MouseEvent event);
    void keyDown(KeyEvent event);
    void keyUp(KeyEvent event);
    void resize(ResizeEvent event);

    void draw();
    void drawWolfram();
    void drawWolframFour();
    void drawGame();
    void drawFlocking();
    void drawLSystem();
    void drawStrange();
    void drawStrange3D();

    void update();
    void updateGame();
    void updateFlocking();
    void updateLSystem();
    void updateStrange3D();

    void initLSystem();

    enum Mode {
        WOLFRAM,
        WOLFRAMFOUR,
        GAME,
        FLOCKING,
        LSYSTEM,
        STRANGE,
        STRANGE3D
    };

    AICamera mCamera;
    bool wDown, aDown, sDown, dDown, qDown, eDown, mPickMode,spaceDown, mMouseMove, mDrawPick, mChainEdit;
    gl::Fbo mFbo; // main framebuffer (AA)
    gl::Light* light;

    Material stepMaterial;
    alife::cell::WolframCA* ca;
    alife::Flock* flock;
    alife::cell::GameOfLife gameOfLife;
    _XDisplay* display;
    Window rootWindow;
    Vec2f mScreenCenter;
    Mode mode;
    bool clear;
    float screenX, screenY;
    int ruleSet;

    // LSystem variables
    alife::LSystem  lSystem;
    geometry::Vec2i  coord,direction;
    std::string sentence;
    int sentenceLength, lSystemLength, lSystemIteration, lSystemScale;
    std::vector< std::pair<geometry::Vec2i, geometry::Vec2i> > stack;

    // Strange Attractor variables
    alife::Pickover2D strange2d;
    alife::Pickover3D strange3d;
};

ArtificialLife::~ArtificialLife()
{
    delete ca;
    delete flock;
    delete light;
    XCloseDisplay(display);
}

void ArtificialLife::prepareSettings(Settings *settings)
{
    screenX = getDisplay().getWidth();
    screenY = getDisplay().getHeight();
    mScreenCenter = Vec2f(screenX / 2, screenY / 2);
    settings->setFrameRate(30.0f);
}

void ArtificialLife::setup()
{
    setFullScreen(true);
    display = XOpenDisplay(0);
    rootWindow = DefaultRootWindow(display);
    XWarpPointer(display, None, rootWindow, 0, 0, 0, 0, mScreenCenter.x, mScreenCenter.y);
    qApp->setOverrideCursor(QCursor(Qt::BlankCursor));
    mMouseMove = false;

    stepMaterial.setAmbient(Color(0.75, 0.75, 0.75));
    stepMaterial.setDiffuse(Color(0.75, 0.75, 0.75));
    stepMaterial.setSpecular(Color(0.25, 0.25, 0.25));
    stepMaterial.setShininess(0.5f);
    stepMaterial.setEmission(Color::white());

    glPointSize(3.0);
    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_MULTISAMPLE);

    // randomize the gameOfLife grid
    for(int i = 0; i < 20; ++i)
    {
        gameOfLife.randomize();
    }

    ca = new alife::cell::WolframCA(ruleSet, 2, 1, screenX/6);

    /* FLOCKING STUFF*/
    flock = new alife::Flock(geometry::Vec3i(-1000, 0, -1000), geometry::Vec3i(1000, 2000, 1000), false, true);


    for(int i = 0; i < 300; ++i)
    {
        flock->addBoid(geometry::Vec3d(alife::randomRange(0, 500), alife::randomRange(0, 500),
                        alife::randomRange(0, 500)));
    }

    light = new gl::Light(gl::Light::SPOTLIGHT, 0);
    light->setAmbient(Color(0.5, 0.5f, 0.5f));
    light->setDiffuse(Color(0.8, 0.8, 0.8));
    light->setSpecular(Color(0.9, 0.9, 0.9));
    light->setAttenuation(2.0, 1.0, 1.0);
}

void ArtificialLife::quit()
{
    cinder::app::AppBasic::quit();
}

void ArtificialLife::keyDown(KeyEvent event)
{	

    switch(event.getCode())
    {
    case KeyEvent::KEY_w:
        wDown = true;
        break;

    case KeyEvent::KEY_a:
        aDown = true;
        break;

    case KeyEvent::KEY_s:
        sDown = true;
        break;

    case KeyEvent::KEY_d:
        dDown = true;
        break;

    case KeyEvent::KEY_q:
        qDown = true;
        break;

    case KeyEvent::KEY_e:
        eDown = true;
        break;

    case KeyEvent::KEY_SPACE:
        spaceDown = true;
        break;

    case KeyEvent::KEY_1:
        mode = WOLFRAM;
        clear = true;
        ruleSet = 50;
        break;

    case KeyEvent::KEY_2:
        mode = WOLFRAMFOUR;
        clear = true;
        ruleSet = 100;
        break;

    case KeyEvent::KEY_3:
        mode = GAME;
        break;

    case KeyEvent::KEY_4:
        mode = FLOCKING;
        break;

    case KeyEvent::KEY_5:
        mode = LSYSTEM;
        initLSystem();
        break;

    case KeyEvent::KEY_6:
        mode = STRANGE;
        clear = true;
        strange2d = alife::Pickover2D(false, 100, alife::randomRange(0,screenX)/screenX, alife::randomRange(0,screenY)/screenY);
        break;

    case KeyEvent::KEY_7:
        mode = STRANGE3D;
        clear = true;
        strange3d = alife::Pickover3D(true, 100, alife::randomRange(0,screenX)/screenX, alife::randomRange(0,screenY)/screenY, alife::randomRange(0,screenY)/screenY);
        break;
    }
}

void ArtificialLife::keyUp(KeyEvent event)
{
    switch(event.getCode())
    {
    case KeyEvent::KEY_w:
        wDown = false;
        break;

    case KeyEvent::KEY_a:
        aDown = false;
        break;

    case KeyEvent::KEY_s:
        sDown = false;
        break;

    case KeyEvent::KEY_d:
        dDown = false;
        break;

    case KeyEvent::KEY_q:
        qDown = false;
        break;

    case KeyEvent::KEY_e:
        eDown = false;
        break;

    case KeyEvent::KEY_SPACE:
        spaceDown = false;
        break;

    case KeyEvent::KEY_ESCAPE:
        quit();
        break;
    }
}

void ArtificialLife::resize(ResizeEvent event)
{
    mCamera.setup(event.getAspectRatio());
}

void ArtificialLife::mouseDown(MouseEvent event)
{

}

void ArtificialLife::mouseUp(MouseEvent event)
{

}

void ArtificialLife::mouseMove(MouseEvent event)
{
    mCamera.mouseInput(event.getPos() - mScreenCenter);
    XSelectInput(display, rootWindow, KeyReleaseMask);
    XWarpPointer(display, None, rootWindow, 0, 0, 0, 0, mScreenCenter.x, mScreenCenter.y);
    XFlush(display);
    mMouseMove = true;
}

void ArtificialLife::update()
{
    switch(mode)
    {
    case FLOCKING:
        updateFlocking();
        break;

    case GAME:
        updateGame();
        break;

    case STRANGE3D:
        updateStrange3D();
        break;

    default:
        break;
    }
}

void ArtificialLife::updateGame()
{
    if(spaceDown)
        gameOfLife.randomize();
    else
        gameOfLife.update();
}

void ArtificialLife::updateFlocking()
{
    mCamera.keyboardInput(wDown, aDown, sDown, dDown, qDown, eDown, spaceDown);
    mCamera.update();

    flock->run();
    cinder::Vec3f target = mCamera.getEye() + (mCamera.getCam().getViewDirection() * 400);

    if(spaceDown)
        flock->seek(geometry::Vec3d(target.x, target.y, target.z));
}

void ArtificialLife::updateStrange3D()
{
    mCamera.keyboardInput(wDown, aDown, sDown, dDown, qDown, eDown, spaceDown);
    mCamera.update();

    for(int i = 0; i < 10; ++i)
    {
        strange3d.update();
    }
}

void ArtificialLife::draw()
{   
    switch(mode)
    {
    case WOLFRAM:
        for(unsigned int i = 0; i < 8; ++i)
        {
            drawWolfram();
        }
        break;

    case WOLFRAMFOUR:
        for(unsigned int i = 0; i < 4; ++i)
        {
            drawWolframFour();
        }
        break;

    case FLOCKING:
        drawFlocking();
        break;

    case GAME:
        drawGame();
        break;

    case LSYSTEM:
        for(unsigned int i = 0; i < 8; ++i)
        {
            drawLSystem();
        }
        break;

    case STRANGE:
        for(int i = 0; i < 30; ++i)
        {
            drawStrange();
        }
        break;

    case STRANGE3D:
        drawStrange3D();
        break;
    }
}

void ArtificialLife::drawWolfram()
{
    if(clear)
    {
        gl::clear(Color::white());
        clear = false;
    }

    gl::color(Color(0, 0, 0));

    const std::vector<unsigned int>& currentGeneration = ca->getCurrentState();

    for(int i = 0; i < currentGeneration.size(); ++i)
    {
        unsigned int state = currentGeneration[i];
        if(state > 0)
        {
            glPushMatrix();
            glTranslatef(i * 6, ca->iteration * 6, 0);

            glBegin(GL_POLYGON);
            glVertex2i(0, 0);
            glVertex2i(6, 0);
            glVertex2i(6, 6);
            glVertex2i(0, 6);
            glEnd();

            glPopMatrix();
        }
    }

    ca->advance();
    if(ca->iteration * 6 > screenY)
    {
        delete ca;
        ruleSet = (ruleSet + 1) % 255;
        ca = new alife::cell::WolframCA(ruleSet, 2, 1, screenX/6);
        clear = true;
    }
}

void ArtificialLife::drawWolframFour()
{
    if(clear)
    {
        gl::clear(Color::white());
        clear = false;
    }

    gl::color(Color(0, 0, 0));

    const std::vector<unsigned int>& currentGeneration = ca->getCurrentState();

    for(int i = 0; i < currentGeneration.size(); ++i)
    {
        unsigned int state = currentGeneration[i];
        if(state > 0)
        {

            switch(state)
            {
            case 1:
                gl::color(Color(0, 0, 0));
                break;

            case 2:
                gl::color(Color(0.5, 0, 0.3));
                break;

            case 3:
                gl::color(Color(0, 0.4, 0.6));
                break;
            }


            glPushMatrix();
            glTranslatef(i * 6, ca->iteration * 6, 0);

            glBegin(GL_POLYGON);
            glVertex2i(0, 0);
            glVertex2i(6, 0);
            glVertex2i(6, 6);
            glVertex2i(0, 6);
            glEnd();

            glPopMatrix();
        }
    }

    ca->advance();
    if(ca->iteration * 6 > screenY)
    {
        delete ca;
        ruleSet += 9;
        ca = new alife::cell::WolframCA(ruleSet, 4, 1, screenX/6);
        clear = true;
    }
}

void ArtificialLife::drawGame()
{
    gl::clear(Color::white());
    glPushMatrix();
    glTranslatef(40, 25, 0);
    gl::color(Color(0, 0, 0));

    for(int y = 0; y < gameOfLife.height; ++y)
    {
        for(int x = 0; x < gameOfLife.width; ++x)
        {
            if(gameOfLife(x, y) == 1)
            {
                glPushMatrix();
                glTranslatef(x * 6, y * 6, 0);

                glBegin(GL_POLYGON);
                glVertex2i(0, 0);
                glVertex2i(6, 0);
                glVertex2i(6, 6);
                glVertex2i(0, 6);
                glEnd();

                glPopMatrix();

            }
        }
    }

    for(int y = 0; y <= gameOfLife.height; ++y)
    {
        glBegin(GL_LINE_STRIP);
        glVertex2i(0, y * 6);
        glVertex2i(gameOfLife.width * 6, y * 6);
        glEnd();
    }

    for(int x = 0; x <= gameOfLife.width; ++x)
    {
        glBegin(GL_LINE_STRIP);
        glVertex2i(x * 6, 0);
        glVertex2i(x * 6, gameOfLife.height * 6);
        glEnd();
    }

    glPopMatrix();
}

void ArtificialLife::drawFlocking()
{
    gl::clear(ColorA(1,1,1,1));
    gl::pushMatrices();
    gl::setMatrices(mCamera.getCam());

    stepMaterial.apply();

    light->setPosition(mCamera.getCam().getEyePoint());
    light->setDirection(mCamera.getCam().getViewDirection());
    light->enable();

    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);
    gl::enableDepthRead();
    gl::enableDepthWrite();

    gl::color(ColorA(0.1,0.35,0.4));

    const std::vector<alife::Boid*>& boids = flock->getBoids();

    for(int i = 0; i < boids.size(); ++i)
    {
        alife::Boid* boid = boids[i];
        geometry::Vec3d location = boid->getLocation();
        gl::drawCube(cinder::Vec3f(location.x, location.y, location.z), cinder::Vec3f(3,3,3));
    }


    gl::color(ColorA(0, 0, 0));
    gl::drawStrokedCube(Vec3f(0,0,0), Vec3f(3000,3000,3000));

    glPopAttrib();
    gl::popMatrices();
}

void ArtificialLife::drawLSystem()
{
    if(clear)
    {
        gl::clear(Color::white());
        clear = false;
    }

    if(++lSystemIteration >= sentenceLength)
    {
        lSystemIteration = 0;
        lSystemScale++;
        coord = geometry::Vec2i(screenX/2, screenY/2);
        lSystemLength = pow(2.0, lSystemScale) / 2;
        if(lSystemScale > 10)
            initLSystem();
    }

    char c = sentence.at(lSystemIteration);

    switch(c)
    {
    case 'F':
    case 'G':

        for(int k = 0; k < lSystemLength; ++k) // draw a line as along as our length
        {
            coord = coord + direction;

            if(coord.x < screenX && coord.x >= 0
                    && coord.y < screenY && coord.y >= 0)
            {
                gl::drawSolidRect(cinder::Rectf(coord.x, coord.y, coord.x + 1, coord.y + 1));
            }
        }

        break;

    case '+':

        // Rotate clockwise
        if(direction == geometry::Vec2i(0, 1))
            direction = geometry::Vec2i(1, 0);

        else if(direction == geometry::Vec2i(1, 0))
                direction = geometry::Vec2i(0, -1);

        else if(direction == geometry::Vec2i(0, -1))
                direction = geometry::Vec2i(-1, 0);

        else if(direction == geometry::Vec2i(-1, 0))
                direction = geometry::Vec2i(0, 1);

        break;

    case '-':

        // Rotate counter clockwise
        if(direction == geometry::Vec2i(0, 1))
            direction = geometry::Vec2i(-1, 0);

        else if(direction == geometry::Vec2i(-1, 0))
                direction = geometry::Vec2i(0, -1);

        else if(direction == geometry::Vec2i(0, -1))
                direction = geometry::Vec2i(1, 0);

        else if(direction == geometry::Vec2i(1, 0))
                direction = geometry::Vec2i(0, 1);

        break;

    case '[': // Push Matrix
        stack.push_back(std::make_pair(coord, direction));
        break;

    case ']': // Pop Matrix

        if(stack.size() > 0)
        {
            coord = stack.back().first;
            direction = stack.back().second;
            stack.pop_back();
        }

        break;

    default:
        std::cout << "Default Char: " << c << std::endl;
    }
}

void ArtificialLife::drawStrange()
{
    if(clear)
    {
        gl::clear(Color::white());
        clear = false;
    }

    gl::color(0, 0, 0, 0.25);
    geometry::Vec2d strangeCoord = strange2d.update() * geometry::Vec2d(200, 200) + geometry::Vec2d(screenX/2, screenY/2);
    gl::drawSolidRect(cinder::Rectf(strangeCoord.x, strangeCoord.y, strangeCoord.x + 1, strangeCoord.y + 1));
}

void ArtificialLife::drawStrange3D()
{
    gl::clear(ColorA(1,1,1,1));
    gl::pushMatrices();
    gl::setMatrices(mCamera.getCam());

    glPushAttrib(GL_CURRENT_BIT | GL_ENABLE_BIT | GL_DEPTH_BUFFER_BIT);
    gl::enableDepthRead();
    gl::enableDepthWrite();

    gl::color(0, 0, 0, 0.5);

    const std::vector<geometry::Vec3d>& history = strange3d.getHistory();
    glBegin(GL_POINTS);
    for(int i = 0; i < history.size(); ++i)
    {
        geometry::Vec3d strangeCoord = history.at(i) * geometry::Vec3d(70, 70, 70) + geometry::Vec3d(0, 0, screenY/2);
        glVertex3d(strangeCoord.x, strangeCoord.y, strangeCoord.z);
    }

    glEnd();

    glPopAttrib();
    gl::popMatrices();
}

void ArtificialLife::initLSystem()
{
    alife::rule_set ruleSet;

    std::string currentRule("F+[+F-F-F]-[-F+F+F]");
    ruleSet.push_back(alife::Rule(alife::alphabet[alife::randomRange(0, 5)], currentRule));
    currentRule.clear();

    for(int i = 0; i < alife::randomRange(30,60); ++i)
    {
        for(int j = 0; j < alife::randomRange(20, 60); ++j)
        {
            currentRule.append(1, alife::alphabet[alife::randomRange(0, 5)]);
        }

        ruleSet.push_back(alife::Rule(alife::alphabet[alife::randomRange(0, 5)], currentRule));
        currentRule.clear();
    }

    currentRule.append(&alife::alphabet[alife::randomRange(0, 5)]);

    for(int j = 0; j < alife::randomRange(30, 60); ++j)
    {
        currentRule.append(1, alife::alphabet[alife::randomRange(0, 5)]);
    }

    lSystem = alife::LSystem(currentRule, ruleSet);

    coord = geometry::Vec2i(screenX/2, screenY/2);

    direction = geometry::Vec2i(alife::randomRange(-1, 1), alife::randomRange(-1, 1));

    stack.clear();
    stack.push_back(std::make_pair(coord, direction));

    if(direction == geometry::Vec2i(0, 0))
        direction = geometry::Vec2i(1, 0);

    lSystem.generate();
    sentence = lSystem.getSentence();
    sentenceLength = sentence.length();
    lSystemScale = 1;
    lSystemLength = 1;
    lSystemIteration = 0;
    clear = true;
}

CINDER_APP_BASIC( ArtificialLife, RendererGl )
