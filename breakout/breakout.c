//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// number of rows of bricks
#define ROWS 5

// number of columns of bricks
#define COLS 10

// radius of ball in pixels
#define RADIUS 10

// lives
#define LIVES 3

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points);
GObject detectCollision(GWindow window, GOval ball);

int main(void)
{
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;
    
    // velocity of ball
    double velocityX = drand48();
    double velocityY = 3.0;

    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {
        // Keeps the paddle on the same x coord. as the mouse
        GEvent event = getNextEvent(MOUSE_EVENT);
        if (event != NULL)
        {
            if (getEventType(event) == MOUSE_MOVED)
            {
                double x = getX(event) - getWidth(paddle) / 2;
                setLocation(paddle, x, HEIGHT - 100);
            }
        }
        
        // Makes the ball move
        move(ball, velocityX, velocityY);
        if (getX(ball) + getWidth(ball) >= getWidth(window))
        {
            velocityX = -velocityX;
        }
        else if (getX(ball) <= 0)
        {
            velocityX = -velocityX;
        }
        
        if (getY(ball) + getHeight(ball) >= getHeight(window) && lives > 0)
        {   
            GEvent event = getNextEvent(MOUSE_EVENT);                
            if (event != NULL)
            {
                if (getEventType(event) == MOUSE_CLICKED)
                {
                    ball = initBall(window);
                    move(ball, velocityX, velocityY);
                    lives--;
                }
            }
        }
        else if (getY(ball) <= 0)
        {
            velocityY = -velocityY;
        }
        
        GObject object = detectCollision(window, ball);
        if (strcmp(getType(object), "GRect") == 0 && object != paddle)
        {
            velocityY = -velocityY;
            removeGWindow(window, object);
            bricks--;
            points++;
            updateScoreboard(window, label, points);
        }
        else if (strcmp(getType(object), "GRect") == 0)
        {
            velocityY = -velocityY;
        }

        pause(10);
    }

    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window)
{
    int brick_width = 35;
    int brick_height = 10;
    int y_pos = 100;
    char* colors[5] = {"RED", "ORANGE", "YELLOW", "GREEN", "BLUE"};
    for (int i = 0; i < ROWS; i++)
    {       
        int y_pos = i * (brick_height + 5) + 50;
        for (int j = 0; j < COLS; j++)
        {
            GRect brick = newGRect(j * (brick_width + 5) + 2, y_pos, brick_width, brick_height);            
            setFilled(brick, true);
            setColor(brick, colors[i]);
            add(window, brick);
        }
    }
}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window)
{
    int ball_width = 25;
    int ball_height = 25;
    GOval ball = newGOval((WIDTH - ball_width) / 2, (HEIGHT - ball_height) / 2, ball_width, ball_height);
    setFilled(ball, true);
    add(window, ball);
    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window)
{
    int paddle_width = 75;
    int paddle_height = 10;
    GRect paddle = newGRect((WIDTH - paddle_width) / 2, HEIGHT - 100, paddle_width, paddle_height);
    setFilled(paddle, true);
    setColor(paddle, "BLACK");
    add(window, paddle);
    
    return paddle;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window)
{
    GLabel label = newGLabel("50");
    setFont(label, "SansSerif-36");
    add(window, label);
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window.
 */
void updateScoreboard(GWindow window, GLabel label, int points)
{
    // update label
    char s[12];
    sprintf(s, "%i", points);
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball)
{
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}
