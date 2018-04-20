/*
Graphics Lab Project
AUST CSE Spring 2017
Level-4 Term-2

Project Name: Formula 1 (3d car model)

Authors:    1. Md. Fahim Mohiuddin
            2. Tahmid-Ur-Rahman
*/
#include<windows.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <GL/glut.h>


#include <assert.h>
#include <fstream>
#include <math.h>
#include "imageloader.h"

using namespace std;

// SIZES
float scaleTest = 0.5f;
// wheels
float wheel_connector_rad = 0.07f;
float front_wheel_connector_angle_height = wheel_connector_rad * 4.0f;
float rear_wheel_connector_angle_height = wheel_connector_rad * 2.0f;
float circle_slice_count = 50.0f;
// front wheel
float front_tire_thickness = 1.0f;
float front_wheel_scale = 0.5f;
float front_wheel_elevation = 0.0f;
float front_wheel_span = 2.3f;
float front_wheel_origin_to = 3.0f;
// rear wheel
float rear_tire_thickness = 1.4f;
float rear_wheel_scale = 0.57f;
float rear_wheel_elevation = 0.07f;
float rear_wheel_span = 2.7f;
float rear_wheel_origin_to = -1.8f;

// body base
float body_base_length = 1.2f;
float body_base_width = 1.2f;
float body_base_height = 0.45f;
float body_base_scale = 1.8f;
float body_base_elevation = 0.1f;
float nose_part_elevation = body_base_height * 0.3f;
float nose_transition_elevation = body_base_height * 0.1f;

// back portion
float body_back_length = body_base_length * 0.45f;
float body_back_width = body_base_width * 0.75f;

// body intake part
float driver_gap = (body_base_length/2.0f) * 0.8f;
float intake_height = body_base_height * 0.7f;
float intake_bottom_width = (body_base_width/2.0f) * 0.6f;
float intake_top_width = intake_bottom_width * 0.7f;
float intake_start_length = (body_base_length/2.0f) * 0.6f;
float intake_end_length = (body_base_length+body_back_length-driver_gap)-intake_start_length;

// rear wing
float rear_wing_length = body_back_length * 0.9f;
float rear_wing_height = rear_wing_length * 0.6f;
float rear_wing_bottom_height = rear_wing_length * 0.3f;
float rear_wing_tilt_angle = 20.0f;

// nose
float nose_length = body_base_length * 0.6f;
float nose_start_width = body_base_width * 0.4f;
float nose_end_width = nose_start_width * 0.7f;
float nose_width_reduce_ratio = (nose_start_width-nose_end_width) / nose_length;
// nose mid
float nose_mid_length = nose_length * 0.5f;
float nose_mid_end_width = nose_end_width - (nose_width_reduce_ratio*nose_mid_length);
float nose_mid_height_reduce = (body_base_height/2.0f) * 0.2f;
// nose last
float nose_last_length = nose_length * 1.0f;
float nose_last_end_width = nose_mid_end_width - (nose_width_reduce_ratio*nose_last_length);
float nose_last_height_reduce = (body_base_height/2.0f) * 1.3f;
// front wing
float front_wing_length = nose_length * 0.5f;
float front_wing_width = (front_wheel_span/2.0f+front_tire_thickness/2.0f)/2.0f;
float front_wing_tip_height = nose_part_elevation * 1.2f;

// track
float track_width = rear_wheel_span * 2.0f;
float bar_width = front_tire_thickness * 0.2f;
float bar_length = front_tire_thickness * 1.0f;
float bar_gap = bar_length * 1.0f;
float track_length = -body_base_length * 70.0f;
float track_repeat_length = track_length * 0.8f;
float bar_position = track_length;
float bar_count = 100.0f;

// track light
float track_light_path_length = body_base_length * 30.0f;
float track_light_height = body_base_height * 5.0f;
float track_light_position = -track_light_path_length;

// STATES
bool isWheelsEnabled = true;
bool isTextureEnabled = true;
bool isLightingEnabled = true;
bool isPositionedLightEnabled = true;
bool isTrackLightEnabled = true;

// Auto Show
bool bannerShow = true;
bool autoShow = true;
float autoShowTilt = 20.0f;
float autoShowAngle = 180.0f;
float autoShowSpeed = 0.4f;

// SCENE VIEW
float camera_angle_UD = 90.0f;
float camera_angle_LR = 0.0f;
float model_position_X = 0.0f;
float model_position_Y = 0.0f;
float model_position_Z = -6.0f;
float model_scale = 0.7f;
float side_light_span = 1.0f;

// motion
float wheel_max_speed = 100.0f, wheel_acceleration = 1.0f;
float wheel_angle = 0.0f, wheel_speed = wheel_max_speed;
float track_speed = 0.0f, track_to_wheel_speed = 0.01f;
bool gas = false;

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
    case 't':
        isTextureEnabled = !isTextureEnabled;
        break;
    case 'q':
        isLightingEnabled = !isLightingEnabled;
        break;
    case 'r':
        isPositionedLightEnabled = !isPositionedLightEnabled;
        break;
    case 'y':
        isTrackLightEnabled = !isTrackLightEnabled;
        break;
    case 'o':
        autoShow = !autoShow;
        break;
    case 'i':
        bannerShow = !bannerShow;
        break;
    case 'n':
        camera_angle_UD += 5.0f;
        break;
    case 'm':
        camera_angle_UD -= 5.0f;
        break;
    case ',':
        camera_angle_LR -= 5.0f;
        break;
    case '.':
        camera_angle_LR += 5.0f;
        break;
    case 'z':
        model_position_X -= 0.5f;
        break;
    case 'x':
        model_position_X += 0.5f;
        break;
    case 'a':
        model_position_Y += 0.5f;
        break;
    case 's':
        model_position_Y -= 0.5f;
        break;
    case 'd':
        model_position_Z -= 0.5f;
        break;
    case 'c':
        model_position_Z += 0.5f;
        break;
    case 'f':
        model_scale += 0.1f;
        break;
    case 'v':
        model_scale -= 0.1f;
        break;
    case 'p':
        gas = true;
        break;
    case 'w':
        isWheelsEnabled = !isWheelsEnabled;
	}
	glutPostRedisplay();
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
				 0,                            //0 for now
				 GL_RGB,                       //Format OpenGL uses for image
				 image->width, image->height,  //Width and height
				 0,                            //The border of the image
				 GL_RGB, //GL_RGB, because pixels are stored in RGB format
				 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
				                   //as unsigned numbers
				 image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

// tire textures
GLuint tire_front_texture, tire_rear_texture;

// body base textures
GLuint body_base_right_texture, body_base_left_texture, body_base_front_texture, body_base_rear_texture, base_top_texture;
// nose textures
GLuint nose_top_ferrari_texture, nose_top_aust_texture, vodafone_texture;
// wing texture
GLuint front_wing_texture, rear_wing_texture, shell_white_texture;
// intake texture
GLuint ferrari_intake_texture, intake_front_texture;
// misc textures
GLuint plain_red_texture, plain_white_texture, plain_gray_texture, car_rear_texture, banner_texture;

GLuint getTexture(const char* filename) {
    Image* image = loadBMP(filename);
	GLuint tex_id = loadTexture(image);
	delete image;
	return tex_id;
}

void setTexture(GLuint tex_id) {
    glBindTexture(GL_TEXTURE_2D,tex_id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
}

void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);

	/*glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);*/
	//glEnable(GL_TEXTURE_2D);
	//glEnable(GL_LIGHTING);

	tire_front_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\tire_front_texture.bmp");
    tire_rear_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\tire_rear_texture.bmp");

    body_base_right_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\body_base_right.bmp");
    body_base_left_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\body_base_left.bmp");
    body_base_front_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\body_base_front.bmp");
    body_base_rear_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\body_base_rear.bmp");
    base_top_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\base_top.bmp");

    nose_top_ferrari_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\nose_top_ferrari.bmp");
    nose_top_aust_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\aust_red_logo.bmp");
    vodafone_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\vodafone_red.bmp");

    front_wing_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\front_wing.bmp");
    rear_wing_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\rear_wing.bmp");
    shell_white_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\shell_white.bmp");

    intake_front_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\intake_front.bmp");
    ferrari_intake_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\ferrari_intake.bmp");

    plain_red_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\plain_red.bmp");
    plain_white_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\plain_white.bmp");
    plain_gray_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\plain_gray.bmp");

    car_rear_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\car_rear.bmp");

    banner_texture = getTexture("H:\\Study\\Lab\\Graphics\\_FINAL PROJECT\\texture_ready to run\\banner.bmp");
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

float angle(float deg) {
    float r = (deg / 180.0f) * 3.141592653589793f;
    return r;
}

// work variables
float x, x_back, y, y_back, angle_per_slice, ttl_angle;

void draw_wheel(float w_scale,float w_tire_thickness,float w_rotation_direction);
void draw_body_base();
void draw_track_bars();
void draw_wheel_connectors(float angle_height, float span, float tire_width);
void draw_track_light();

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // SHOW BANNER
    if(bannerShow) {
        glDisable(GL_LIGHTING);
        glEnable(GL_TEXTURE_2D);
        glColor3f(1.0f,1.0f,1.0f);
        setTexture(banner_texture);
        glBegin(GL_QUADS);
        glTexCoord2f(1.0f,1.0f);
        glVertex3f(2.0f,1.0f,-5.0f);
        glTexCoord2f(0.0f,1.0f);
        glVertex3f(-2.0f,1.0f,-5.0f);
        glTexCoord2f(0.0f,0.0f);
        glVertex3f(-2.0f,-1.0f,-5.0f);
        glTexCoord2f(1.0f,0.0f);
        glVertex3f(2.0f,-1.0f,-5.0f);
        glEnd();
        glutSwapBuffers();
        return;
    }

    // Enable/Disable Texture
    if(isTextureEnabled) glEnable(GL_TEXTURE_2D);
    else glDisable(GL_TEXTURE_2D);
    // Enable/Disable Lighting
    if(isLightingEnabled) {
        glEnable(GL_LIGHTING);
        if(isPositionedLightEnabled) {
            // right side light
            glEnable(GL_LIGHT0);
            GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
            GLfloat lightPos0[] = {side_light_span/2.0f, 0.0f, 0.0f, 1.0f}; //Positioned at (4, 0, 8)
            glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
            glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
            // left side light
            glEnable(GL_LIGHT1);
            GLfloat lightColor1[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
            GLfloat lightPos1[] = {-side_light_span/2.0f, 0.0f, 0.0f, 1.0f}; //Positioned at (4, 0, 8)
            glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
            glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
            // top light
            glEnable(GL_LIGHT2);
            GLfloat lightColor2[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
            GLfloat lightPos2[] = {0.0f, side_light_span/2.0f, 0.0f, 1.0f}; //Positioned at (4, 0, 8)
            glLightfv(GL_LIGHT2, GL_DIFFUSE, lightColor2);
            glLightfv(GL_LIGHT2, GL_POSITION, lightPos2);
            // track light (enable only)
            glEnable(GL_LIGHT3);
        } else {
            glDisable(GL_LIGHT0);
            glDisable(GL_LIGHT1);
            glDisable(GL_LIGHT2);
            glDisable(GL_LIGHT3);
        }
    } else glDisable(GL_LIGHTING);

	// MAIN TRANSLATION AND ROTATION
	if(autoShow) {
        // AUTO MODE
        // Model Position
        glTranslatef(0.0f, 0.0f, -6.0f);
        // Model Roation
        glRotatef(autoShowTilt,1.0f,0.0f,0.0f);
        glRotatef(-autoShowAngle,0.0f,1.0f,0.0f);
	} else {
        // MANUAL MODE
        // Model Position
        glTranslatef(model_position_X, model_position_Y, model_position_Z);
        // Model Roation
        glRotatef(camera_angle_UD,1.0f,0.0f,0.0f);
        glRotatef(camera_angle_LR,0.0f,1.0f,0.0f);
	}

    // Model Scale
    glScalef(model_scale,model_scale,model_scale);

    // track
    {
    glDisable(GL_LIGHTING);
    glPushMatrix();
    glRotatef(-90,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,0.0f,-(body_base_height/2.0f+body_base_elevation*3.0f));
    // right side
    glTranslatef(track_width/2.0f,0.0f,0.0f);
    draw_track_bars();
    // left side
    glTranslatef(-track_width,0.0f,0.0f);
    draw_track_bars();
    glPopMatrix();
    if(isLightingEnabled) glEnable(GL_LIGHTING);
    }

    // DRAW TRACK LIGHT
    {
    glPushMatrix();
    glRotatef(-90,1.0f,0.0f,0.0f);
    glTranslatef(0.0f,0.0f,track_light_height);
    if(isTrackLightEnabled) {
        draw_track_light();
    }
    glPopMatrix();
    }

    // body base
    {
    glPushMatrix();
    glTranslatef(0.0f,body_base_elevation,0.0f);
    glRotatef(-90,1.0f,0.0f,0.0f);
    draw_body_base();
    glPopMatrix();
    }

    // wheels
    if(isWheelsEnabled) {
        // front right wheel
        glPushMatrix();
        glTranslatef(front_wheel_span/2.0,front_wheel_elevation,front_wheel_origin_to);
        glRotatef(90,0.0f,1.0f,0.0f);
        draw_wheel_connectors(0.0f,front_wheel_span/2.0f,front_tire_thickness);
        draw_wheel_connectors(front_wheel_connector_angle_height,front_wheel_span/2.0f,front_tire_thickness);
        draw_wheel(front_wheel_scale,front_tire_thickness,1.0f);
        glPopMatrix();
        // front left wheel
        glPushMatrix();
        glTranslatef(-front_wheel_span/2.0,front_wheel_elevation,front_wheel_origin_to);
        glRotatef(-90,0.0f,1.0f,0.0f);
        draw_wheel_connectors(0.0f,front_wheel_span/2.0f,front_tire_thickness);
        draw_wheel_connectors(front_wheel_connector_angle_height,front_wheel_span/2.0f,front_tire_thickness);
        draw_wheel(front_wheel_scale,front_tire_thickness,-1.0f);
        glPopMatrix();

        // rear right wheel
        glPushMatrix();
        glTranslatef(rear_wheel_span/2.0,rear_wheel_elevation,rear_wheel_origin_to);
        glRotatef(90,0.0f,1.0f,0.0f);
        draw_wheel_connectors(0.0f,rear_wheel_span/2.0f,rear_tire_thickness);
        draw_wheel_connectors(rear_wheel_connector_angle_height,rear_wheel_span/2.0f,rear_tire_thickness);
        draw_wheel(rear_wheel_scale,rear_tire_thickness,1.0f);
        glPopMatrix();
        // rear left wheel
        glPushMatrix();
        glTranslatef(-rear_wheel_span/2.0,rear_wheel_elevation,rear_wheel_origin_to);
        glRotatef(-90,0.0f,1.0f,0.0f);
        draw_wheel_connectors(0.0f,rear_wheel_span/2.0f,rear_tire_thickness);
        draw_wheel_connectors(rear_wheel_connector_angle_height,rear_wheel_span/2.0f,rear_tire_thickness);
        draw_wheel(rear_wheel_scale,rear_tire_thickness,-1.0f);
        glPopMatrix();
    }

	glutSwapBuffers();
}

void draw_track_light() {
    // track light
    float intensity = 1.0f - (fabs(track_light_position)/track_light_path_length);
    GLfloat lightColor3[] = {intensity, intensity, intensity, 1.0f}; //Color (0.5, 0.5, 0.5)
    GLfloat lightPos3[] = {0.0f, track_light_position, 0.0f, 1.0f}; //Positioned at (4, 0, 8)
    glLightfv(GL_LIGHT3, GL_DIFFUSE, lightColor3);
    glLightfv(GL_LIGHT3, GL_POSITION, lightPos3);
}

void draw_track_bars() {
    for(float i=0.0f; i<bar_count; i++) {
        float this_bar_pos = bar_position + i*(bar_length+bar_gap);
        glColor3f(1.0f,1.0f,1.0f);
        glBegin(GL_QUADS);
        glVertex3f(bar_width/2.0f,this_bar_pos+bar_length,0.0f);
        glVertex3f(-bar_width/2.0f,this_bar_pos+bar_length,0.0f);
        glVertex3f(-bar_width/2.0f,this_bar_pos,0.0f);
        glVertex3f(bar_width/2.0f,this_bar_pos,0.0f);
        glEnd();
    }
}

void draw_body_base() {
    glPushMatrix(); // 1
    glScalef(body_base_scale,body_base_scale,body_base_scale);

    // main body base
    {
    // top part
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,0.0f,1.0f);  // pink top part
    setTexture(base_top_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,0.0f,1.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(body_base_width/2.0f,body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-body_base_width/2.0f,body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-body_base_width/2.0f,-body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(body_base_width/2.0f,-body_base_length/2.0f,body_base_height/2.0f);
    glEnd();
    // right side
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,0.0f,0.0f);
    setTexture(body_base_right_texture);
    glBegin(GL_QUADS);
    glNormal3f(1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(body_base_width/2.0f,body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(body_base_width/2.0f,-body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(body_base_width/2.0f,-body_base_length/2.0f,-body_base_height/2.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(body_base_width/2.0f,body_base_length/2.0f,-body_base_height/2.0f);
    glEnd();
    // left side
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,0.0f,0.0f);
    setTexture(body_base_left_texture);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-body_base_width/2.0f,-body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-body_base_width/2.0f,body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-body_base_width/2.0f,body_base_length/2.0f,-body_base_height/2.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-body_base_width/2.0f,-body_base_length/2.0f,-body_base_height/2.0f);
    glEnd();
    // front side
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(0.0f,0.0f,1.0f);
    setTexture(body_base_front_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,-1.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(body_base_width/2.0f,-body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-body_base_width/2.0f,-body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-body_base_width/2.0f,-body_base_length/2.0f,-body_base_height/2.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(body_base_width/2.0f,-body_base_length/2.0f,-body_base_height/2.0f);
    glEnd();
    // back side
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(0.0f,0.0f,1.0f);
    setTexture(body_base_rear_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,1.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-body_base_width/2.0f,body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(body_base_width/2.0f,body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(body_base_width/2.0f,body_base_length/2.0f,-body_base_height/2.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-body_base_width/2.0f,body_base_length/2.0f,-body_base_height/2.0f);
    glEnd();
    // bottom part [NOT ORDERED]
    glColor3f(1.0f,1.0f,1.0f);
    glBegin(GL_QUADS);
    //glNormal3f(0.0f,0.0f,-1.0f);
    glVertex3f(body_base_width/2.0f,body_base_length/2.0f,-body_base_height/2.0f);
    glVertex3f(-body_base_width/2.0f,body_base_length/2.0f,-body_base_height/2.0f);
    glVertex3f(-body_base_width/2.0f,-body_base_length/2.0f,-body_base_height/2.0f);
    glVertex3f(body_base_width/2.0f,-body_base_length/2.0f,-body_base_height/2.0f);
    glEnd();
    }

    glPushMatrix(); // M1
    glTranslatef(0.0f,-body_base_length/2.0f+driver_gap,body_base_height/2.0f);

    // body intake part
    {
    // intake front
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,0.0f,0.0f);
    setTexture(intake_front_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,-1.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(intake_top_width/2.0f,0.0f,intake_height);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-intake_top_width/2.0f,0.0f,intake_height);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-intake_bottom_width/2.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(intake_bottom_width/2.0f,0.0f,0.0f);
    glEnd();
    // intake start right
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,1.0f,0.0f);
    setTexture(ferrari_intake_texture);
    glBegin(GL_QUADS);
    glNormal3f(1.0f,0.0f,0.3f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(intake_top_width/2.0f,intake_start_length,intake_height);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(intake_top_width/2.0f,0.0f,intake_height);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(intake_bottom_width/2.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(intake_bottom_width/2.0f,intake_start_length,0.0f);
    glEnd();
    // intake start left
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,1.0f,0.0f);
    setTexture(ferrari_intake_texture);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f,0.0f,0.3f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-intake_top_width/2.0f,0.0f,intake_height);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-intake_top_width/2.0f,intake_start_length,intake_height);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-intake_bottom_width/2.0f,intake_start_length,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-intake_bottom_width/2.0f,0.0f,0.0f);
    glEnd();
    // intake start top
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,1.0f,0.0f);
    setTexture(plain_red_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,0.0f,1.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(intake_top_width/2.0f,intake_start_length,intake_height);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-intake_top_width/2.0f,intake_start_length,intake_height);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-intake_top_width/2.0f,0.0f,intake_height);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(intake_top_width/2.0f,0.0f,intake_height);
    glEnd();

    glPushMatrix(); // M2
    glTranslatef(0.0,intake_start_length,0.0f);
    // intake end right
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(0.0f,1.0f,0.0f);
    setTexture(plain_red_texture);
    glBegin(GL_TRIANGLES);
    glNormal3f(1.0f,0.2f,0.3f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(intake_top_width/2.0f,intake_end_length,0.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(intake_top_width/2.0f,0.0f,intake_height);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(intake_bottom_width/2.0f,0.0f,0.0f);
    // intake end left
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(0.0f,1.0f,0.0f);
    setTexture(plain_red_texture);
    glBegin(GL_TRIANGLES);
    glNormal3f(-1.0f,0.2f,0.3f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-intake_top_width/2.0f,0.0f,intake_height);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-intake_top_width/2.0f,intake_end_length,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-intake_bottom_width/2.0f,0.0f,0.0f);
    glEnd();
    // intake end top
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(0.0f,1.0f,0.0f);
    setTexture(plain_red_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,0.4f,1.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(intake_top_width/2.0f,intake_end_length,0.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-intake_top_width/2.0f,intake_end_length,0.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-intake_top_width/2.0f,0.0f,intake_height);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(intake_top_width/2.0f,0.0f,intake_height);
    glEnd();
    }

    glPopMatrix(); // M2
    glPopMatrix(); // M1

    glPushMatrix(); // B1
    glTranslatef(0.0f,body_base_length/2.0f,body_base_height/2.0f);

    // body back part
    {
    // back top
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,0.0f,1.0f);  // pink top part
    setTexture(plain_red_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,0.0f,1.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(body_back_width/2.0f,body_back_length,0.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-body_back_width/2.0f,body_back_length,0.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-body_back_width/2.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(body_back_width/2.0f,0.0f,0.0f);
    glEnd();
    // back right
    glColor3f(1.0f,1.0f,1.0f);
    setTexture(plain_red_texture);
    glBegin(GL_QUADS);
    glNormal3f(1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(body_back_width/2.0f,body_back_length,0.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(body_back_width/2.0f,0.0f,0.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(body_back_width/2.0f,0.0f,-body_base_height);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(body_back_width/2.0f,body_back_length,-body_base_height);
    glEnd();
    // back left
    glColor3f(1.0f,1.0f,1.0f);
    setTexture(plain_red_texture);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-body_back_width/2.0f,0.0f,0.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-body_back_width/2.0f,body_back_length,0.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-body_back_width/2.0f,body_back_length,-body_base_height);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-body_back_width/2.0f,0.0f,-body_base_height);
    glEnd();
    // back rear
    glColor3f(1.0f,1.0f,1.0f);
    setTexture(car_rear_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,1.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-body_back_width/2.0f,body_back_length,0.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(body_back_width/2.0f,body_back_length,0.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(body_back_width/2.0f,body_back_length,-body_base_height);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-body_back_width/2.0f,body_back_length,-body_base_height);
    glEnd();
    }

    glPushMatrix(); // B2
    glTranslatef(0.0f,body_back_length-rear_wing_length/2.0f,0.0f);

    // rear wing part
    {
    // rear wing right bottom
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(0.0f,1.0f,1.0f);
    setTexture(plain_white_texture);
    glBegin(GL_QUADS);
    glNormal3f(1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(body_back_width/2.0f,rear_wing_length,rear_wing_bottom_height);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(body_back_width/2.0f,0.0f,rear_wing_bottom_height);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(body_back_width/2.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(body_back_width/2.0f,rear_wing_length/2.0f,0.0f);
    glEnd();
    // rear wing right top
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(0.0f,0.0f,1.0f);
    setTexture(shell_white_texture);
    glBegin(GL_QUADS);
    glNormal3f(1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(body_back_width/2.0f,rear_wing_length,rear_wing_height+rear_wing_bottom_height);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(body_back_width/2.0f,0.0f,rear_wing_height+rear_wing_bottom_height);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(body_back_width/2.0f,0.0f,rear_wing_bottom_height);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(body_back_width/2.0f,rear_wing_length,rear_wing_bottom_height);
    glEnd();
    // rear wing left bottom
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(0.0f,1.0f,1.0f);
    setTexture(plain_white_texture);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-body_back_width/2.0f,0.0f,rear_wing_bottom_height);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-body_back_width/2.0f,rear_wing_length,rear_wing_bottom_height);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-body_back_width/2.0f,rear_wing_length/2.0f,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-body_back_width/2.0f,0.0f,0.0f);
    glEnd();
    // rear wing left top
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(0.0f,0.0f,1.0f);
    setTexture(shell_white_texture);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-body_back_width/2.0f,0.0f,rear_wing_height+rear_wing_bottom_height);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-body_back_width/2.0f,rear_wing_length,rear_wing_height+rear_wing_bottom_height);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-body_back_width/2.0f,rear_wing_length,rear_wing_bottom_height);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-body_back_width/2.0f,0.0f,rear_wing_bottom_height);
    glEnd();

    // rear wing top part
    glPushMatrix(); // B3
    glTranslatef(0.0f,rear_wing_length,rear_wing_bottom_height+rear_wing_height);
    glRotatef(rear_wing_tilt_angle,1.0f,0.0f,0.0f);
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,0.0f,0.0f);
    setTexture(rear_wing_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,0.0f,1.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(body_back_width/2.0f,0.0f,0.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-body_back_width/2.0f,0.0f,0.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-body_back_width/2.0f,-rear_wing_length,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(body_back_width/2.0f,-rear_wing_length,0.0f);
    glEnd();
    glPopMatrix(); // B3
    }

    glPopMatrix(); //B2
    glPopMatrix(); //B1

    // nose starting part
    {
    // nose top
    glColor3f(1.0f,1.0f,1.0f);
    setTexture(nose_top_ferrari_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,0.0f,1.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(nose_start_width/2.0f,-body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-nose_start_width/2.0f,-body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-nose_end_width/2.0f,-body_base_length/2.0f-nose_length,body_base_height/2.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(nose_end_width/2.0f,-body_base_length/2.0f-nose_length,body_base_height/2.0f);
    glEnd();
    // nose right
    glColor3f(1.0f,1.0f,1.0f);
    setTexture(vodafone_texture);
    glBegin(GL_QUADS);
    glNormal3f(1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(nose_start_width/2.0f,-body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(nose_end_width/2.0f,-body_base_length/2.0f-nose_length,body_base_height/2.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(nose_end_width/2.0f,-body_base_length/2.0f-nose_length,-(body_base_height/2.0f-nose_part_elevation));
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(nose_start_width/2.0f,-body_base_length/2.0f,-(body_base_height/2.0f-nose_transition_elevation));
    glEnd();
    // nose left
    glColor3f(1.0f,1.0f,1.0f);
    setTexture(vodafone_texture);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-nose_end_width/2.0f,-body_base_length/2.0f-nose_length,body_base_height/2.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-nose_start_width/2.0f,-body_base_length/2.0f,body_base_height/2.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-nose_start_width/2.0f,-body_base_length/2.0f,-(body_base_height/2.0f-nose_transition_elevation));
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-nose_end_width/2.0f,-body_base_length/2.0f-nose_length,-(body_base_height/2.0f-nose_part_elevation));
    glEnd();
    }

    glPushMatrix(); // 2
    glTranslatef(0.0f,-body_base_length/2.0f-nose_length,body_base_height/2.0f);

    // nose mid part
    {
    // nose mid top
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,1.0f,0.0f);
    setTexture(nose_top_aust_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,0.0f,1.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(nose_end_width/2.0f,0.0f,0.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-nose_end_width/2.0f,0.0f,0.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-nose_mid_end_width/2.0f,-nose_mid_length,-nose_mid_height_reduce);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(nose_mid_end_width/2.0f,-nose_mid_length,-nose_mid_height_reduce);
    glEnd();
    // nose mid right
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,1.0f,0.0f);
    setTexture(plain_red_texture);
    glBegin(GL_QUADS);
    glNormal3f(1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(nose_end_width/2.0f,0.0f,0.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(nose_mid_end_width/2.0f,-nose_mid_length,-nose_mid_height_reduce);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(nose_mid_end_width/2.0f,-nose_mid_length,-(body_base_height-nose_part_elevation));
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(nose_end_width/2.0f,0.0f,-(body_base_height-nose_part_elevation));
    glEnd();
    // nose mid left
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,1.0f,0.0f);
    setTexture(plain_red_texture);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-nose_mid_end_width/2.0f,-nose_mid_length,-nose_mid_height_reduce);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-nose_end_width/2.0f,0.0f,0.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-nose_end_width/2.0f,0.0f,-(body_base_height-nose_part_elevation));
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-nose_mid_end_width/2.0f,-nose_mid_length,-(body_base_height-nose_part_elevation));
    glEnd();
    }

    glPushMatrix(); // 3
    glTranslatef(0.0f,-nose_mid_length,0.0f);

    // nose last part
    {
    // nose last top
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(0.0f,1.0f,1.0f);
    setTexture(plain_red_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,0.0f,1.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(nose_mid_end_width/2.0f,0.0f,-nose_mid_height_reduce);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-nose_mid_end_width/2.0f,0.0f,-nose_mid_height_reduce);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-nose_last_end_width/2.0f,-nose_last_length,-nose_last_height_reduce);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(nose_last_end_width/2.0f,-nose_last_length,-nose_last_height_reduce);
    glEnd();
    // nose last right
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(0.0f,1.0f,1.0f);
    setTexture(plain_red_texture);
    glBegin(GL_QUADS);
    glNormal3f(1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(nose_mid_end_width/2.0f,0.0f,-nose_mid_height_reduce);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(nose_last_end_width/2.0f,-nose_last_length,-nose_last_height_reduce);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(nose_last_end_width/2.0f,-nose_last_length,-(body_base_height-nose_part_elevation));
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(nose_mid_end_width/2.0f,0.0f,-(body_base_height-nose_part_elevation));
    glEnd();
    // nose last left
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(0.0f,1.0f,1.0f);
    setTexture(plain_red_texture);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-nose_last_end_width/2.0f,-nose_last_length,-nose_last_height_reduce);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-nose_mid_end_width/2.0f,0.0f,-nose_mid_height_reduce);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-nose_mid_end_width/2.0f,0.0f,-(body_base_height-nose_part_elevation));
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-nose_last_end_width/2.0f,-nose_last_length,-(body_base_height-nose_part_elevation));
    glEnd();
    /*// nose last front
    glColor3f(0.0f,1.0f,0.0f);
    glBegin(GL_QUADS);
    glVertex3f(nose_last_end_width/2.0f,-nose_last_length,-nose_last_height_reduce);
    glVertex3f(-nose_last_end_width/2.0f,-nose_last_length,-nose_last_height_reduce);
    glVertex3f(-nose_last_end_width/2.0f,-nose_last_length,-body_base_height);
    glVertex3f(nose_last_end_width/2.0f,-nose_last_length,-body_base_height);
    glEnd();*/
    }

    glPushMatrix(); // 4
    glTranslatef(0.0f,-nose_last_length,-body_base_height);

    // front wing part
    {
    // front wing main part
    glColor3f(1.0f,1.0f,1.0f);
    setTexture(front_wing_texture);
    glBegin(GL_QUADS);
    glNormal3f(0.0f,0.0f,1.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(front_wing_width,front_wing_length,0.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-front_wing_width,front_wing_length,0.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-front_wing_width,0.0f,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(front_wing_width,0.0f,0.0f);
    glEnd();
    // front wing right support
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,0.0f,0.0f);
    setTexture(plain_white_texture);
    glBegin(GL_QUADS);
    glNormal3f(1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(nose_last_end_width/2.0,front_wing_length,nose_part_elevation);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(nose_last_end_width/2.0,0.0f,nose_part_elevation);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(nose_last_end_width/2.0,0.0f,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(nose_last_end_width/2.0,front_wing_length,0.0f);
    glEnd();
    // front wing right tip
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,0.0f,0.0f);
    //setTexture(plain_red_texture);
    setTexture(vodafone_texture);
    glBegin(GL_QUADS);
    glNormal3f(1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(front_wing_width,front_wing_length,front_wing_tip_height);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(front_wing_width,0.0f,front_wing_tip_height);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(front_wing_width,0.0f,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(front_wing_width,front_wing_length,0.0f);
    glEnd();
    // front wing left support
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,0.0f,0.0f);
    setTexture(plain_white_texture);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-nose_last_end_width/2.0,0.0f,nose_part_elevation);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-nose_last_end_width/2.0,front_wing_length,nose_part_elevation);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-nose_last_end_width/2.0,front_wing_length,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-nose_last_end_width/2.0,0.0f,0.0f);
    glEnd();
    // front wing left tip
    if(isTextureEnabled) {
        glColor3f(1.0f,1.0f,1.0f);
    } else glColor3f(1.0f,0.0f,0.0f);
    //setTexture(plain_red_texture);
    setTexture(vodafone_texture);
    glBegin(GL_QUADS);
    glNormal3f(-1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3f(-front_wing_width,0.0f,front_wing_tip_height);
    glTexCoord2f(0.0f,1.0f);
    glVertex3f(-front_wing_width,front_wing_length,front_wing_tip_height);
    glTexCoord2f(0.0f,0.0f);
    glVertex3f(-front_wing_width,front_wing_length,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3f(-front_wing_width,0.0f,0.0f);
    glEnd();
    }

    glPopMatrix(); // 4
    glPopMatrix(); // 3
    glPopMatrix(); // 2

    glPopMatrix(); // 1
}

void draw_wheel(float w_scale,float w_tire_thickness,float w_rotation_direction) {
    // Wheel
    glPushMatrix();
    // translation
    // scale
    glScalef(w_scale,w_scale,w_scale);
    // wheel motion rotation
    glRotatef(wheel_angle*w_rotation_direction,0.0f,0.0f,1.0f);
    // angle
    angle_per_slice = 360.0f / circle_slice_count;
    ttl_angle = angle_per_slice;
    // points
    x_back = 1.0f;
    y_back = 0.0f;
    for(int i=0; i<circle_slice_count; i++) {
        // calculate coordinates
        x = cos(angle(ttl_angle));
        y = sin(angle(ttl_angle));

        // front face
        // color
        glColor3f(1.0f,1.0f,1.0f);
        // texture
        setTexture(tire_front_texture);
        // draw
        glBegin(GL_TRIANGLES);
        glTexCoord2f(0.5f+x*0.5f,0.5f+y*0.5f);
        glNormal3f(x,y,w_tire_thickness/2.0f);
        glVertex3f(x,y,w_tire_thickness/2.0f);
        glTexCoord2f(0.5f+x_back*0.5f,0.5f+y_back*0.5f);
        glNormal3f(x_back,y_back,w_tire_thickness/2.0f);
        glVertex3f(x_back,y_back,w_tire_thickness/2.0f);
        glTexCoord2f(0.5f+0.0f,0.5f+0.0f);
        glNormal3f(0.0f,0.0f,w_tire_thickness/2.0f);
        glVertex3f(0.0f,0.0f,w_tire_thickness/2.0f);
        glEnd();

        // filling
        // color
        glColor3f(0.1f,0.1f,0.1f);
        // draw
        glBegin(GL_QUADS);
        glNormal3f(x,y,w_tire_thickness/2.0f);
        glVertex3f(x,y,w_tire_thickness/2.0f);
        glNormal3f(x_back,y_back,w_tire_thickness/2.0f);
        glVertex3f(x_back,y_back,w_tire_thickness/2.0f);
        glNormal3f(x_back,y_back,-w_tire_thickness/2.0f);
        glVertex3f(x_back,y_back,-w_tire_thickness/2.0f);
        glNormal3f(x,y,-w_tire_thickness/2.0f);
        glVertex3f(x,y,-w_tire_thickness/2.0f);
        glEnd();

        // back face
        // color
        glColor3f(1.0f,1.0f,1.0f);
        // texture
        setTexture(tire_rear_texture);
        // draw
        glBegin(GL_TRIANGLES);
        glTexCoord2f(0.5f+x*0.5f,0.5f+y*0.5f);
        glNormal3f(x,y,-w_tire_thickness/2.0f);
        glVertex3f(x,y,-w_tire_thickness/2.0f);
        glTexCoord2f(0.5f+x_back*0.5f,0.5f+y_back*0.5f);
        glNormal3f(x_back,y_back,-w_tire_thickness/2.0f);
        glVertex3f(x_back,y_back,-w_tire_thickness/2.0f);
        glTexCoord2f(0.5f+0.0f,0.5f+0.0f);
        glNormal3f(0.0f,0.0f,-w_tire_thickness/2.0f);
        glVertex3f(0.0f,0.0f,-w_tire_thickness/2.0f);
        glEnd();

        // update coordinates
        x_back = x;
        y_back = y;
        // update angle
        ttl_angle += angle_per_slice;
    }
    glPopMatrix();
}

void draw_wheel_connectors(float angle_height, float span, float tire_width) {
    //span -= tire_width/2.0f;
    glPushMatrix();
    //glTranslatef(0.0f,0.0f,-tire_width/2.0f);
    // front
    glColor3f(1.0f,1.0f,1.0f);
    setTexture(plain_gray_texture);
    glBegin(GL_QUADS);
    glTexCoord2f(1.0f,1.0f);
    glVertex3d(wheel_connector_rad/2.0f,wheel_connector_rad/2.0f,0.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3d(-wheel_connector_rad/2.0f,wheel_connector_rad/2.0f,0.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3d(-wheel_connector_rad/2.0f,-wheel_connector_rad/2.0f,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3d(wheel_connector_rad/2.0f,-wheel_connector_rad/2.0f,0.0f);
    glEnd();
    // right
    glBegin(GL_QUADS);
    glNormal3f(1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3d(wheel_connector_rad/2.0f,wheel_connector_rad/2.0f+angle_height,-span);
    glTexCoord2f(0.0f,1.0f);
    glVertex3d(wheel_connector_rad/2.0f,wheel_connector_rad/2.0f,0.0f);
    glTexCoord2f(0.0f,0.0f);
    glVertex3d(wheel_connector_rad/2.0f,-wheel_connector_rad/2.0f,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3d(wheel_connector_rad/2.0f,-wheel_connector_rad/2.0f+angle_height,-span);
    glEnd();
    // left
    glBegin(GL_QUADS);
    glNormal3f(-1.0f,0.0f,0.0f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3d(-wheel_connector_rad/2.0f,wheel_connector_rad/2.0f,0.0f);
    glTexCoord2f(0.0f,1.0f);
    glVertex3d(-wheel_connector_rad/2.0f,wheel_connector_rad/2.0f+angle_height,-span);
    glTexCoord2f(0.0f,0.0f);
    glVertex3d(-wheel_connector_rad/2.0f,-wheel_connector_rad/2.0f+angle_height,-span);
    glTexCoord2f(1.0f,0.0f);
    glVertex3d(-wheel_connector_rad/2.0f,-wheel_connector_rad/2.0f,0.0f);
    glEnd();
    // top
    glBegin(GL_QUADS);
    glNormal3f(0.0f,1.0f,0.3f);
    glTexCoord2f(1.0f,1.0f);
    glVertex3d(wheel_connector_rad/2.0f,wheel_connector_rad/2.0f+angle_height,-span);
    glTexCoord2f(0.0f,1.0f);
    glVertex3d(-wheel_connector_rad/2.0f,wheel_connector_rad/2.0f+angle_height,-span);
    glTexCoord2f(0.0f,0.0f);
    glVertex3d(-wheel_connector_rad/2.0f,wheel_connector_rad/2.0f,0.0f);
    glTexCoord2f(1.0f,0.0f);
    glVertex3d(wheel_connector_rad/2.0f,wheel_connector_rad/2.0f,0.0f);
    glEnd();
    glPopMatrix();
}

void update(int value) {
    if(bannerShow) {
        glutPostRedisplay();
        glutTimerFunc(10, update, 0);
        return;
    }

    // update wheel speed
    if(gas) {
        wheel_speed += wheel_acceleration;
        if(wheel_speed>wheel_max_speed) {
            wheel_speed = wheel_max_speed;
        }
    } else {
        wheel_speed -= wheel_acceleration/2.5f;
        if(wheel_speed<0.0f) {
            wheel_speed = 0.0f;
        }
    }
    gas = false;

    // update track position
    track_speed = wheel_speed * track_to_wheel_speed;
    bar_position += track_speed;
    if(bar_position>=track_repeat_length) {
        bar_position = track_length + (bar_position-track_repeat_length);
    }

    // update track light position
    track_light_position += track_speed;
    if(track_light_position>=track_light_path_length) {
        track_light_position = -track_light_path_length;
    }

    // update wheel rotation
    wheel_angle += wheel_speed;
    if(wheel_angle>360.f) {
        wheel_angle -= 360.f;
    }

    // update auto show
    autoShowAngle += autoShowSpeed;
    if(autoShowAngle>=360.0f) {
        autoShowAngle -= 360.0f;
    }

	glutPostRedisplay();
	glutTimerFunc(10, update, 0);
}


int main(int argc, char** argv) {
	/*printf("%f\n",sin(angle(0.0f)));
	printf("%f\n",sin(angle(45.0f)));
	printf("%f\n",sin(angle(90.0f)));*/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);

	glutCreateWindow("Formula 1");
	initRendering();

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);

	glutTimerFunc(10, update, 0);

	glutMainLoop();
	return 0;
}
Image::Image(char* ps, int w, int h) : pixels(ps), width(w), height(h) {

}

Image::~Image() {
	delete[] pixels;
}

namespace {
	//Converts a four-character array to an integer, using little-endian form
	int toInt(const char* bytes) {
		return (int)(((unsigned char)bytes[3] << 24) |
					 ((unsigned char)bytes[2] << 16) |
					 ((unsigned char)bytes[1] << 8) |
					 (unsigned char)bytes[0]);
	}

	//Converts a two-character array to a short, using little-endian form
	short toShort(const char* bytes) {
		return (short)(((unsigned char)bytes[1] << 8) |
					   (unsigned char)bytes[0]);
	}

	//Reads the next four bytes as an integer, using little-endian form
	int readInt(ifstream &input) {
		char buffer[4];
		input.read(buffer, 4);
		return toInt(buffer);
	}

	//Reads the next two bytes as a short, using little-endian form
	short readShort(ifstream &input) {
		char buffer[2];
		input.read(buffer, 2);
		return toShort(buffer);
	}

	//Just like auto_ptr, but for arrays
	template<class T>
	class auto_array {
		private:
			T* array;
			mutable bool isReleased;
		public:
			explicit auto_array(T* array_ = NULL) :
				array(array_), isReleased(false) {
			}

			auto_array(const auto_array<T> &aarray) {
				array = aarray.array;
				isReleased = aarray.isReleased;
				aarray.isReleased = true;
			}

			~auto_array() {
				if (!isReleased && array != NULL) {
					delete[] array;
				}
			}

			T* get() const {
				return array;
			}

			T &operator*() const {
				return *array;
			}

			void operator=(const auto_array<T> &aarray) {
				if (!isReleased && array != NULL) {
					delete[] array;
				}
				array = aarray.array;
				isReleased = aarray.isReleased;
				aarray.isReleased = true;
			}

			T* operator->() const {
				return array;
			}

			T* release() {
				isReleased = true;
				return array;
			}

			void reset(T* array_ = NULL) {
				if (!isReleased && array != NULL) {
					delete[] array;
				}
				array = array_;
			}

			T* operator+(int i) {
				return array + i;
			}

			T &operator[](int i) {
				return array[i];
			}
	};
}

Image* loadBMP(const char* filename) {
	ifstream input;
	input.open(filename, ifstream::binary);
	assert(!input.fail() || !"Could not find file");
	char buffer[2];
	input.read(buffer, 2);
	assert(buffer[0] == 'B' && buffer[1] == 'M' || !"Not a bitmap file");
	input.ignore(8);
	int dataOffset = readInt(input);

	//Read the header
	int headerSize = readInt(input);
	int width;
	int height;
	switch(headerSize) {
		case 40:
			//V3
			width = readInt(input);
			height = readInt(input);
			input.ignore(2);
			assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
			assert(readShort(input) == 0 || !"Image is compressed");
			break;
		case 12:
			//OS/2 V1
			width = readShort(input);
			height = readShort(input);
			input.ignore(2);
			assert(readShort(input) == 24 || !"Image is not 24 bits per pixel");
			break;
		case 64:
			//OS/2 V2
			assert(!"Can't load OS/2 V2 bitmaps");
			break;
		case 108:
			//Windows V4
			assert(!"Can't load Windows V4 bitmaps");
			break;
		case 124:
			//Windows V5
			assert(!"Can't load Windows V5 bitmaps");
			break;
		default:
			assert(!"Unknown bitmap format");
	}

	//Read the data
	int bytesPerRow = ((width * 3 + 3) / 4) * 4 - (width * 3 % 4);
	int size = bytesPerRow * height;
	auto_array<char> pixels(new char[size]);
	input.seekg(dataOffset, ios_base::beg);
	input.read(pixels.get(), size);

	//Get the data into the right format
	auto_array<char> pixels2(new char[width * height * 3]);
	for(int y = 0; y < height; y++) {
		for(int x = 0; x < width; x++) {
			for(int c = 0; c < 3; c++) {
				pixels2[3 * (width * y + x) + c] =
					pixels[bytesPerRow * y + 3 * x + (2 - c)];
			}
		}
	}

	input.close();
	return new Image(pixels2.release(), width, height);
}










