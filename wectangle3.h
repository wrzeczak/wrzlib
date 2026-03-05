//-----------------------------------------------------------------------------//
// WECTANGLE - wrzRectangle, a rotatable Raylib Rectangle-style rectangle with //
// collision and texturing. Written for and by wrzeczak 2025.                  //
//-----------------------------------------------------------------------------//

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <raylib.h>
#include <raymath.h>

//------------------------------------------------------------------------------

/**
 * A Raylib-compatible rotateable Rectangle. W for wrzeczak.
 * 
 * @param x, y, width, height Equivalent to the Raylib `Rectangle`'s fields. Position is the top-left corner of a Rectangle/Wectangle with `rot` of `0.0f`.
 * @param rot Rotation, **in degrees**, about the center of the Wectangle.
 */
typedef struct {
    float x;
    float y;
    float width;
    float height;
    float rot;
} Wectangle;

//------------------------------------------------------------------------------

void DrawWectangle(float posX, float posY, float width, float height, float rot, Color color);
void DrawWectangleV(Vector2 position, Vector2 size, float rot, Color color);
void DrawWectangleRec(Rectangle rec, float rot, Color color);
void DrawWectangleWec(Wectangle wec, Color color);
void DrawWectangleLines(Wectangle wec, Color color);
void DrawWectangleLinesEx(Wectangle wec, float lineThick, Color color);

void DrawWectangleTex(Wectangle wec, Texture2D tex, Color tint);

Wectangle WectangleZero();
Vector2 WectanglePos(Wectangle wec);
Vector2 WectangleAxis(Wectangle wec);
Vector2 WectangleDim(Wectangle wec);
float WectangleChord(Wectangle wec);

/**
 * The triangles of a Wectangle.
 * 
 * @param a1, a2, a3 One of two triangles. Contains duplicate data to triangle b. Should be in counter-clockwise (renderable) order.
 * @param b1, b2, b3 The other of the two triangles.
 */
typedef struct {
    Vector2 a1, a2, a3;
    Vector2 b1, b2, b3;
} WectTris;

WectTris WectangleTris(Wectangle wec);

/**
 * The corners of a Wectangle. No guarantees about the ordering of them. Use `WectangleTris()` to get a counter-clockwise (renderable) set of points.
 * 
 * @param a, b, c, d `Vector2` uniquely representing a corner.
 */
typedef struct {
    Vector2 a, b, c, d;
} WecCorners;

WecCorners WectangleCorners(Wectangle wec);

bool CheckCollisionPointWec(Vector2 a, Wectangle b);
bool CheckCollisionWecs(Wectangle a, Wectangle b);
bool CheckCollisionWecRec(Wectangle a, Rectangle b);

/**
 * A dynamic list of collision points between two Wectangles (or a Wectangle and a Rectangle). `GetCollisionWec()` performs `malloc()` on `tris`, be sure to `free()`!
 * 
 * @param points A **potentially NULL** list of `Vector2` that represents in (hopefully; this is still W.I.P) counter-clockwise order the up to eight points where two rectangles can collide. If `num_points` is zero, this will be `NULL`.
 * @param num_points The number of points in `points`. If this is zero, points is `NULL`.
 */
typedef struct {
    Vector2 * points;
    unsigned int num_points;
} WecCollision;

WecCollision GetCollisionWecs(Wectangle a, Wectangle b);
WecCollision GetCollisionWecRec(Wectangle a, Rectangle b);

float WecCollisionArea(WecCollision coll);

Wectangle TranslateWectangle(Wectangle wec, Vector2 trans);

//------------------------------------------------------------------------------
// BASIC SHAPE DRAWING - RAYLIB API IMITATION

/**
 * Draw a Wectangle to the screen.
 * 
 * @param posX x-position of the top-left corner if rot were 0.
 * @param posY y-position of the top-left corner if rot were 0.
 * @param width the length parallel to the bottom of the screen if rot were 0.
 * @param height the length perpendicular to the bottom of the screen if rot were 0.
 * @param rot the rotation, **in degrees**, if the Wectangle about its center.
 */
void DrawWectangle(float posX, float posY, float width, float height, float rot, Color color) {
    WectTris tris = WectangleTris((Wectangle) { .x = posX, .y = posY, .width = width, .height = height, .rot = rot });

    if(((int) roundf(rot) % 90) != 0) {
        DrawTriangle(tris.a1, tris.a2, tris.a3, color);
        DrawTriangle(tris.b1, tris.b2, tris.b3, color);
    } else if(((int) roundf(rot) % 180) == 0) {
        DrawRectangle(posX, posY, width, height, color);
    } else {
        Vector2 pos = (tris.a3.y < tris.b1.y) ? tris.a3 : tris.b1;
        DrawRectangle(pos.x, pos.y, height, width, color);
    }
    
}

/**
 * Draw a Wectangle to the screen. See `DrawWectangle()`
 */
void DrawWectangleV(Vector2 position, Vector2 size, float rot, Color color) {
    DrawWectangle(position.x, position.y, size.x, size.y, rot, color);
}

/**
 * Draw a Wectangle to the screen. See `DrawWectangle()`
 */
void DrawWectangleRec(Rectangle rec, float rot, Color color) {
    DrawWectangle(rec.x, rec.y, rec.width, rec.height, rot, color);
}

/**
 * Draw a Wectangle to the screen. See `DrawWectangle()`
 */
void DrawWectangleWec(Wectangle wec, Color color) {
    DrawWectangle(wec.x, wec.y, wec.width, wec.height, wec.rot, color);
}

/**
 * Draw a Wectangle to the screen. See `DrawWectangle()`. Instead of drawing a solid color, this draws an outline.
 */
void DrawWectangleLines(Wectangle wec, Color color) {
    DrawWectangleLinesEx(wec, 1.0f, color);
}

/**
 * Draw a Wectangle's outline to the screen, with extra parameters.
 */
void DrawWectangleLinesEx(Wectangle wec, float lineThick, Color color) {
    WectTris tris = WectangleTris(wec);

    if(((int) roundf(wec.rot) % 90) != 0) {
        DrawLineV(tris.a3, tris.a2, color);
        DrawLineV(tris.a2, tris.a1, color);
        DrawLineV(tris.b2, tris.b1, color);
        DrawLineV(tris.b1, tris.b3, color);
    } else if(((int) roundf(wec.rot) % 180) == 0) {
        // i have to do this transform because it seems DrawRectangleLinesEx() seems to draw lines on-center,
        // but DrawLineEx seems to draw them not on-center, or vice versa? The DR() Rectangles are smaller.
        DrawRectangleLinesEx((Rectangle) { wec.x - (lineThick / 2.0f), wec.y - (lineThick / 2.0f), wec.width + lineThick, wec.height + lineThick }, lineThick, color);
    } else {
        Vector2 pos = (tris.a3.y < tris.b1.y) ? tris.a3 : tris.b1;
        DrawRectangleLinesEx((Rectangle) { pos.x - (lineThick / 2.0f), pos.y - (lineThick / 2.0f), wec.height + lineThick, wec.width + lineThick }, lineThick, color);
    }
}

//----------------------------
// TEXTURE DRAWING

/**
 * Draw a texture within a Wectangle. This will scale the texture of the bounds of the Wectangle, and rotate it accordingly.
 */
void DrawWectangleTex(Wectangle wec, Texture2D tex, Color tint) {
    int posX = wec.x;
    int posY = wec.y;
    int width = wec.width;
    int height = wec.height;
    float rot = fmodf(wec.rot, 360.0f);

    WecCorners corners = WectangleCorners(wec);

    Vector2 a0 = corners.a;
    Vector2 a1 = corners.b;
    Vector2 a2 = corners.c;
    Vector2 a3 = corners.d;

    Vector2 points[4] = { a0, a1, a2, a3 };

    // temporary init - these change
    Vector2 lc = a0;
    Vector2 rc = a1;
    Vector2 tc = a2;
    Vector2 bc = a3;

    for(int i = 0; i < 4; i++) {
        Vector2 p = points[i];

        if(p.x <= lc.x)
            lc = p;
        
        if(p.x >= rc.x)
            rc = p;

        if(p.y <= tc.y)
            tc = p;
        
        if(p.y >= bc.y)
            bc = p;
    }

    // when the rectangle is at a flat angle (90., 180, 270, 360),
    // the vertices in clockwise order are tc, rc, bc, lc (hopefully?)
    /*
            tc-----rc
             |     |
            lc-----bc
    */

    // tex origin
    Vector2 to = lc;

    if(rot > 270.0f && rot < 360.0f) {
        to = lc;
    } else if(rot > 0.0f && rot < 90.0f) {
        to = tc;
    } else if(rot > 90.0f && rot < 180.0f) {
        to = rc;
    } else if(rot > 180.0f && rot <= 270.0f) {
        to = bc;
    }

    if(rot == 0.0f) {
        to.y -= wec.height;
    } else if(rot == 90.0f) {
        to.x += wec.height;
    } else if(rot == 180.0f) {
        to.y += wec.height;
        to.x += wec.width;
    } else if(rot == 270.0f) {
        to.x -= wec.height;
    }

    DrawTexturePro(tex, (Rectangle) { 0, 0, tex.width, tex.height }, (Rectangle) { to.x, to.y, wec.width, wec.height }, Vector2Zero(), wec.rot, tint);
}

//----------------------------
// GETTERS

/**
 * Returns an empty Wectangle.
 * 
 * @return `(Wectangle) { 0, 0, 0, 0, 0.0f };`
 */
inline Wectangle WectangleZero() {
    return (Wectangle) { 0, 0, 0, 0, 0.0f };
}

/**
 * Get a Wectangle from a Rectangle (rot is set to 0.0f)
 * 
 * @param rec The Rectangle to copy.
 * @return `(Wectangle) { rec.x, rec.y, rec.width, rec.height, 0.0f };`
 */
inline Wectangle WectangleFromRec(Rectangle rec) {
    return (Wectangle) { rec.x, rec.y, rec.width, rec.height, 0.0f };
}

/**
 * Get the position of a Wectangle.
 * 
 * @param wec The Wectangle in question.
 * @return `(Vector2) { wec.x, wec.y };`
 */
inline Vector2 WectanglePos(Wectangle wec) {
    return (Vector2) { wec.x, wec.y };
}

/**
 * Get the center of a Wectangle i.e. the axis about which it rotates.
 * 
 * @param wec The Wectangle in question.
 * @return `return (Vector2) { wec.x + (0.5f * wec.width), wec.y + (0.5f * wec.height) };`
 */
inline Vector2 WectangleAxis(Wectangle wec) {
    return (Vector2) { wec.x + (0.5f * wec.width), wec.y + (0.5f * wec.height) };
}

/**
 * Get the width and height of a Wectangle.
 * 
 * @param wec The Wectangle in question.
 * @return `(Vector2) { wec.width, wec.height };`
 */
Vector2 WectangleDim(Wectangle wec) {
    return (Vector2) { wec.width, wec.height };
}

/**
 * Get the longest distance between two of the Wectangle's corners.
```
*----*
|\   | The chord is
| \  | the line drawn
|  \ | with slashes
|   \| in this diagram.
*----*
```
 * @param wec The Wectangle in question.
 * @return The length of the chord.
 */
float WectangleChord(Wectangle wec) {
    return Vector2Distance(WectangleAxis(wec), Vector2Add(WectangleAxis(wec), Vector2Scale(WectangleDim(wec), 0.5f)));
}

/**
 * Get the two triangles that make up a Wectangle. These two triangles (see `WectTris`) should be ordered counter-clockwise (renderable).
 * 
 * @param wec The Wectangle in question.
 * @return See `WectTris`.
 */
WectTris WectangleTris(Wectangle wec) {
    int posX = wec.x;
    int posY = wec.y;
    int width = wec.width;
    int height = wec.height;
    float rot = wec.rot;

    WecCorners corners = WectangleCorners(wec);

    Vector2 a0 = corners.a;
    Vector2 a1 = corners.b;
    Vector2 a2 = corners.c;
    Vector2 a3 = corners.d;

    Vector2 points[4] = { a0, a1, a2, a3 };

    // temporary init - these change
    Vector2 lc = a0;
    Vector2 rc = a1;
    Vector2 tc = a2;
    Vector2 bc = a3;

    for(int i = 0; i < 4; i++) {
        Vector2 p = points[i];

        if(p.x <= lc.x)
            lc = p;
        
        if(p.x >= rc.x)
            rc = p;

        if(p.y <= tc.y)
            tc = p;
        
        if(p.y >= bc.y)
            bc = p;
    }

    return (WectTris) { rc, tc, lc, bc, rc, lc };
}

/**
 * Get the corners of a Wectangle in no particular order.
 * 
 * @param wec The Wectangle in question.
 * @return See `WecCorners`.
 */
WecCorners WectangleCorners(Wectangle wec) {
    int posX = wec.x;
    int posY = wec.y;
    int width = wec.width;
    int height = wec.height;
    float rot = wec.rot;

    // p = prior to rotation
    Vector2 p0 = (Vector2) { posX, posY };
    Vector2 p1 = (Vector2) { posX + width, posY };
    Vector2 p2 = (Vector2) { posX + width, posY + height };
    Vector2 p3 = (Vector2) { posX, posY + height };

    Vector2 center = WectangleAxis(wec);

    // a = after rotation
    Vector2 a0 = Vector2Add(Vector2Rotate(Vector2Subtract(p0, center), DEG2RAD * rot), center);
    Vector2 a1 = Vector2Add(Vector2Rotate(Vector2Subtract(p1, center), DEG2RAD * rot), center);
    Vector2 a2 = Vector2Add(Vector2Rotate(Vector2Subtract(p2, center), DEG2RAD * rot), center);
    Vector2 a3 = Vector2Add(Vector2Rotate(Vector2Subtract(p3, center), DEG2RAD * rot), center);

    return (WecCorners) { a0, a1, a2, a3 };
}

/**
 * Check if there is a collision between a Wectangle and a Point (i.e. if that point is within the Wectangle). This works by checking if the line between the point and the center of the Wectangle  intersects any of the faces of the Wectangle. If it does, the point must be outside the Wectangle.
 * 
 * @param wec The Wectangle in question.
 * @param point The Point in question.
 * @return Whether or not `point` is inside `wec`.
 */
bool CheckCollisionPointWec(Vector2 point, Wectangle wec) {
    WecCorners corners = WectangleCorners(wec);
    Vector2 axis = WectangleAxis(wec);
    
    if(CheckCollisionLines(corners.a, corners.b, point, axis, NULL)) return false;
    if(CheckCollisionLines(corners.b, corners.c, point, axis, NULL)) return false;
    if(CheckCollisionLines(corners.c, corners.d, point, axis, NULL)) return false;
    if(CheckCollisionLines(corners.d, corners.a, point, axis, NULL)) return false;
    return true;
}

/**
 * Get where a Circle and a Wectangle collide. This will return the first scanned point that is within the Wectangle.
 * 
 * @param c The center of the Circle in question.
 * @param radius The radius of the Circle in question.
 * @param wec The Wectangle in question.
 * @param output If not `NULL`, the first point found on the Circle within the Wectangle. If no collision, this is `NULL`.
 * @return Check `output` for return value.
 */
void GetCollisionCircleWec(Vector2 c, float radius, Wectangle wec, Vector2 * output) {
    for(unsigned int i = 0; i < 20; i++) {
        float theta = (i / 20.0f) * 2 * PI;
        Vector2 check = Vector2Add(Vector2Scale((Vector2) { cosf(theta), sinf(theta) }, radius), c);
        if(CheckCollisionPointWec(check, wec)) {
            *output = check;
            return;
        } 
    }

    output = NULL; // use an output value here because simply returning Vector2Zero() would be ambiguous.
}

/**
 * Check whether a Circle and a Wectangle collide.
 * 
 * @param c The center of the Circle in question.
 * @param radius The radius of the Circle in question.
 * @param wec The Wectangle in question.
 * @return A boolean; whether or not `GetCollisionCircleWec(c, radius, wec, check)` sets `check` to `NULL`.
 */
bool CheckCollisionCircleWec(Vector2 c, float radius, Wectangle wec) {
    Vector2 * check = NULL;
    GetCollisionCircleWec(c, radius, wec, check);
   return (check != NULL);
}

/**
 * Check whether two Wectangles are colliding.
 * 
 * @param a, b The Wectangles in question.
 * @return Whether or not they collide.
 */
bool CheckCollisionWecs(Wectangle a, Wectangle b) {
    a = (Wectangle) { a.x, a.y, a.width, a.height, (fmodf(a.rot, 90.0f) == 0.0f) ? a.rot + 0.02f : a.rot };
    b = (Wectangle) { b.x, b.y, b.width, b.height, (fmodf(b.rot, 90.0f) == 0.0f) ? b.rot + 0.02f : b.rot };

    WecCorners a_corners = WectangleCorners(a);
    WecCorners b_corners = WectangleCorners(b);

    if(CheckCollisionPointWec(a_corners.a, b)) return true;
    if(CheckCollisionPointWec(a_corners.b, b)) return true;
    if(CheckCollisionPointWec(a_corners.c, b)) return true;
    if(CheckCollisionPointWec(a_corners.d, b)) return true;
    
    if(CheckCollisionPointWec(b_corners.a, a)) return true;
    if(CheckCollisionPointWec(b_corners.b, a)) return true;
    if(CheckCollisionPointWec(b_corners.c, a)) return true;
    if(CheckCollisionPointWec(b_corners.d, a)) return true;

    if(CheckCollisionLines(a_corners.a, a_corners.b, b_corners.a, b_corners.b, NULL)) return true;
    if(CheckCollisionLines(a_corners.b, a_corners.c, b_corners.a, b_corners.b, NULL)) return true;
    if(CheckCollisionLines(a_corners.c, a_corners.d, b_corners.a, b_corners.b, NULL)) return true;
    if(CheckCollisionLines(a_corners.d, a_corners.a, b_corners.a, b_corners.b, NULL)) return true;

    if(CheckCollisionLines(a_corners.a, a_corners.b, b_corners.b, b_corners.c, NULL)) return true;
    if(CheckCollisionLines(a_corners.b, a_corners.c, b_corners.b, b_corners.c, NULL)) return true;
    if(CheckCollisionLines(a_corners.c, a_corners.d, b_corners.b, b_corners.c, NULL)) return true;
    if(CheckCollisionLines(a_corners.d, a_corners.a, b_corners.b, b_corners.c, NULL)) return true;

    if(CheckCollisionLines(a_corners.a, a_corners.b, b_corners.c, b_corners.d, NULL)) return true;
    if(CheckCollisionLines(a_corners.b, a_corners.c, b_corners.c, b_corners.d, NULL)) return true;
    if(CheckCollisionLines(a_corners.c, a_corners.d, b_corners.c, b_corners.d, NULL)) return true;
    if(CheckCollisionLines(a_corners.d, a_corners.a, b_corners.c, b_corners.d, NULL)) return true;

    if(CheckCollisionLines(a_corners.a, a_corners.b, b_corners.d, b_corners.a, NULL)) return true;
    if(CheckCollisionLines(a_corners.b, a_corners.c, b_corners.d, b_corners.a, NULL)) return true;
    if(CheckCollisionLines(a_corners.c, a_corners.d, b_corners.d, b_corners.a, NULL)) return true;
    if(CheckCollisionLines(a_corners.d, a_corners.a, b_corners.d, b_corners.a, NULL)) return true;

    return false;
}

/**
 * Check whether or not a Wectangle and a Rectangle are colliding.
 * 
 * @param a The Wectangle in question.
 * @param b The Rectangle in question.
 * @return `CheckCollisionWecs(a, WectangleFromRec(b));`
 */
inline bool CheckCollisionWecRec(Wectangle a, Rectangle b) {
    return CheckCollisionWecs(a, WectangleFromRec(b));
}

/**
 * qsort-compatible function for comparing the counterclockwiseness of two points relative to a given center point.
 * 
 * @param vector2_a, vector2_b The points in question. Passed as `(void *) ((const Vector2 *) v)` by `qsort()`.
 * @param centroid The center-point in question. Passed as `(void *) ((const Vector2 *) v)` by `qsort()`.
 * @return qsort-format, checking counterclockwiseness.
 */
int qs_counterclockwise(void * centroid, const void * vector2_a, const void * vector2_b) {
    Vector2 a = *((const Vector2 *) vector2_a);
    Vector2 b = *((const Vector2 *) vector2_b);
    Vector2 c = *((const Vector2 *) centroid);

    float ax = a.x - c.x;
    float ay = a.y - c.y;
    float bx = b.x - c.x;
    float by = b.y - c.y;
    
    float angle_a = PI + atan2f(ay, ax);
    float angle_b = PI + atan2f(by, bx);

    if(angle_a < angle_b) return -1;
    if(angle_a > angle_b) return 1;

    float distance_a = Vector2Distance(*((const Vector2 *) vector2_a), *((const Vector2 *) centroid));
    float distance_b = Vector2Distance(*((const Vector2 *) vector2_b), *((const Vector2 *) centroid));
    if(distance_a > distance_b) return -1;
    if(distance_a < distance_b) return 1;

    return 0;
}

/**
 * Get the collision points of two Wectangles.
 * 
 * @param a, b The Wectangles in question.
 * @return See `WecCollision`.
 */
WecCollision GetCollisionWecs(Wectangle a, Wectangle b) {
    a = (Wectangle) { a.x, a.y, a.width, a.height, (fmodf(a.rot, 90.0f) == 0.0f) ? a.rot + 0.02f : a.rot };
    b = (Wectangle) { b.x, b.y, b.width, b.height, (fmodf(b.rot, 90.0f) == 0.0f) ? b.rot + 0.02f : b.rot };
    WecCorners a_corners = WectangleCorners(a);
    WecCorners b_corners = WectangleCorners(b);

    static Vector2 corners[8] = { 0 };
    unsigned int corners_idx = 0;

    if(!CheckCollisionWecs(a, b)) return (WecCollision) { 0 };

    #define POINTC(field, wec) if(CheckCollisionPointWec(a_corners.field, wec)) { corners[corners_idx] = a_corners.field; corners_idx++; }

    POINTC(a, b);
    POINTC(b, b);
    POINTC(c, b);
    POINTC(d, b);

    #undef POINTC
    #define POINTC(field, wec) if(CheckCollisionPointWec(b_corners.field, wec)) { corners[corners_idx] = b_corners.field; corners_idx++; }

    POINTC(a, a);
    POINTC(b, a);
    POINTC(c, a);
    POINTC(d, a);

    #undef POINTC

    Vector2 coll = Vector2Zero();
    #define EDGEC(x, y, z, w) if(CheckCollisionLines(a_corners.x, a_corners.y, b_corners.z, b_corners.w, &coll)) { corners[corners_idx] = coll; corners_idx++; }
    
    EDGEC(a, b, a, b);
    EDGEC(b, c, a, b);
    EDGEC(c, d, a, b);
    EDGEC(d, a, a, b);
    EDGEC(a, b, b, c);
    EDGEC(b, c, b, c);
    EDGEC(c, d, b, c);
    EDGEC(d, a, b, c);
    EDGEC(a, b, c, d);
    EDGEC(b, c, c, d);
    EDGEC(c, d, c, d);
    EDGEC(d, a, c, d);
    EDGEC(a, b, d, a);
    EDGEC(b, c, d, a);
    EDGEC(c, d, d, a);
    EDGEC(d, a, d, a);

    #undef EDGEC

    if(corners_idx == 0) return (WecCollision) { 0 };

    Vector2 centroid = Vector2Zero();

    for(unsigned int i = 0; i < corners_idx; i++) {
        centroid = Vector2Add(centroid, corners[i]);
    }
    centroid = Vector2Scale(centroid, 1.0f / (float) corners_idx);

    qsort_s(corners, corners_idx, sizeof(Vector2), &qs_counterclockwise, &centroid);

    static WecCollision output = { corners, 0 };
    output.num_points = corners_idx;

    return output;
}

/**
 * Get the collision points between a Wectangle and a Rectangle.
 * 
 * @param a The Wectangle in question.
 * @param b The Rectangle in question.
 * @return `GetCollisionWecs(a, WectangleFromRec(b));`
 */
inline WecCollision GetCollisionWecRec(Wectangle a, Rectangle b) {
    return GetCollisionWecs(a, WectangleFromRec(b));
}

//! **DOES NOT WORK**
float WecCollisionArea(WecCollision coll) {
    if(coll.num_points < 3) return 0.0f;

    float xs[coll.num_points + 1];
    for(unsigned int i = 0; i < coll.num_points; i++) xs[i] = coll.points[i].x;
    xs[coll.num_points] = coll.points[0].x;

    float ys[coll.num_points + 1];
    for(unsigned int i = 0; i < coll.num_points; i++) ys[i] = coll.points[i].y;
    ys[coll.num_points] = coll.points[0].y;

    float output = 0.0f;
    for(unsigned int i = 0; i < coll.num_points; i++) output += (xs[i] * ys[i + 1]);
    for(unsigned int i = 0; i < coll.num_points; i++) output -= (xs[i + 1] * ys[i]);
    return output * 0.5f;
}

//----------------------------
// SETTERS

/**
 * Translate a Wectangle.
 * 
 * @param wec The Wectangle in question.
 * @param trans The amount to translate it.
 * @return Wectangle whose position equals `wec.position + trans`.
 */
inline Wectangle TranslateWectangle(Wectangle wec, Vector2 trans) {
    return (Wectangle) { .x = wec.x + trans.x, .y = wec.y + trans.y, .width = wec.width, .height = wec.height, .rot = wec.rot };
}

//------------------------------------------------------------------------------
// DEBUG FUNCTIONS

void debugDrawWectangle(int posX, int posY, int width, int height, float rot, Color color) {
    // p = prior to rotation
    Vector2 p0 = (Vector2) { posX, posY };
    Vector2 p1 = (Vector2) { posX + width, posY };
    Vector2 p2 = (Vector2) { posX + width, posY + height };
    Vector2 p3 = (Vector2) { posX, posY + height };

    Vector2 unrotated_points[4] = { p0, p1, p2, p3 };

    Vector2 center = (Vector2) { posX + (width * 0.5f ), posY + (height * 0.5f) };

    // a = after rotation
    Vector2 a0 = Vector2Add(Vector2Rotate(Vector2Subtract(p0, center), DEG2RAD * rot), center);
    Vector2 a1 = Vector2Add(Vector2Rotate(Vector2Subtract(p1, center), DEG2RAD * rot), center);
    Vector2 a2 = Vector2Add(Vector2Rotate(Vector2Subtract(p2, center), DEG2RAD * rot), center);
    Vector2 a3 = Vector2Add(Vector2Rotate(Vector2Subtract(p3, center), DEG2RAD * rot), center);

    Vector2 points[4] = { a0, a1, a2, a3 };

    Color colors[4] = { RED, BLUE, GREEN, YELLOW };

    for(int i = 0; i < 4; i++)
        DrawLineV(unrotated_points[i], unrotated_points[(i + 1) % 4], Fade(colors[i], 0.2f));

    for(int i = 0; i < 4; i++)
        DrawLineV(points[i], points[(i + 1) % 4], colors[i]);

    DrawCircleV(center, 5, WHITE);
    DrawLineV(center, a0, WHITE);

    // temporary init - these change
    Vector2 lc = a0;
    Vector2 rc = a1;
    Vector2 tc = a2;
    Vector2 bc = a3;

    Color other_colors[4] = { PINK, SKYBLUE, LIME, GOLD };

    for(int i = 0; i < 4; i++) {
        Vector2 p = points[i];

        if(p.x <= lc.x)
            lc = p;
        
        if(p.x >= rc.x)
            rc = p;

        if(p.y <= tc.y)
            tc = p;
        
        if(p.y >= bc.y)
            bc = p;
    }

    Vector2 corners[4] = { lc, rc, tc, bc };

    for(int i = 0; i < 4; i++) {
        DrawCircleV(corners[i], 3, other_colors[i]);
        DrawLineV(Vector2Subtract(corners[i], (Vector2) { 0, 100 }), Vector2Add(corners[i], (Vector2) { 0, 100 }), Fade(other_colors[i], 0.4f));
    }

    if(((int) roundf(rot) % 90) != 0) {
        DrawTriangle(rc, tc, lc, Fade(color, 0.4f));
        DrawTriangle(bc, rc, lc, Fade(color, 0.4f));
    } else if(((int) roundf(rot) % 180) == 0) {
        DrawRectangle(posX, posY, width, height, Fade(color, 0.6f));
    } else {
        Vector2 pos = (lc.y < bc.y) ? lc : bc;
        DrawRectangle(pos.x, pos.y, height, width, Fade(color, 0.6f));
    }
    
}