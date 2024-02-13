# pragma once

#include "Math.h"
#include "../ObjectMgr/ObjectMgr.h"
#include "olcSprite.h"
#include "../irl/Keyboard.h"
#include "RunTimeEditor.h"
#include <thread>

class Program {
public:
    Math math;
    Mesh mesh;
    mat4x4 matProj;
    vec3D vCamera;
    vec3D vLookDir;
    vec3D vForward;
    Keyboard keyboard;
    mat4x4 matWorld, matTrans, matRotZ, matRotX;
    RunTimeEditor editor;
    Triangle triProjected;
    Triangle triDrawingProjected;
    ObjectMgr objMgr{mesh};
    Drawing drawObj;

    sf::Vector3f Position = objMgr.getPosition(mesh);

    float fTheta = 0;
	bool editMode = true;

	void Initialize() {
		loadMesh(objMgr, mesh);
		createProjectionMatrix(math, matProj);
		matTrans = math.Matrix_MakeTranslation(Position.x, Position.y, Position.z);

        if (editor.EDITOR_STATE != 0)
            TakeInputNewThread();
	}

    void clearTriVec(std::vector<Triangle>& tris) {
        tris.clear();
    }


    void HandleKeyPresses() {
        keyboard.handleMovementKeys(vCamera, math, vForward, editor.isDrawMode);
        keyboard.handleKeyBinds(editor.drawWireFrames);
    }

    void UpdatevForward() {
        vForward = math.Vector_Mul(vLookDir, 0.2f);
    }

	void Update() {

        vec3D vUp = { 0, 1, 0 };
        vec3D vTarget = { 0,0,1 };
        mat4x4 matCameraRot = math.Matrix_MakeRotationY(keyboard.fYaw);
        vLookDir = math.Matrix_MultiplyVector(matCameraRot, vTarget);
        vTarget = math.Vector_Add(vCamera, vLookDir);



        // fTheta += .01f * 0.0003f;
        matRotZ = math.Matrix_MakeRotationZ(fTheta);
        matRotX = math.Matrix_MakeRotationX(fTheta);

        matWorld = math.Matrix_MakeIdentity();
        matWorld = math.Matrix_MultiplyMatrix(matRotZ, matRotX);
        matWorld = math.Matrix_MultiplyMatrix(matWorld, matTrans);


        mat4x4 matCamera = math.Matrix_PointAt(vCamera, vTarget, vUp);

        // Make view matrix from camera
        mat4x4 matView = math.Matrix_QuickInverse(matCamera);

        // main object refers to the object the camera will rotate around, this is passed to the 
        // handleEverything function to tell it if it was called by mesh.tris or drawObj.drawing
        // if it is true then it is the mainObj (mesh.tris), if it false it is the drawing
        // bool mainObj = false;


        // Translate
        if (!editor.isDrawMode) {
            clearTriVec(objMgr.vecTrianglesToRaster);
            // clearTriVec(drawObj.drawing);
            // clearTriVec(drawObj.drawing);
        }

        if (editor.moveObject) {
            sf::Vector3f newPos = editor.newObjPos();
            objMgr.addToMeshPos(mesh, newPos);
            Position = objMgr.getPosition(mesh);
            matTrans = math.Matrix_MakeTranslation(Position.x, Position.y, Position.z);
            TakeInputNewThread();
            editor.moveObject = false;
        }

        if (editor.setObjectPos) {
            sf::Vector3f newPos = editor.newObjPos();
            objMgr.setMeshNewPos(mesh, newPos);
            Position = objMgr.getPosition(mesh);
            matTrans = math.Matrix_MakeTranslation(Position.x, Position.y, Position.z);
            TakeInputNewThread();
            editor.setObjectPos = false;
        }

        if (editor.drawingModeInput != 0)
        {
            TakeInputNewThread();
            editor.drawingModeInput = 0;      
        } 

        if (editor.saveDrawing)
        {  
            // std::cout << "Inside Save Drawing" << std::endl;
            drawObj.saveDrawing(objMgr.vecTrianglesToRaster);
            // drawObj.loadDrawing(objMgr.vecTrianglesToRaster);
            // handleEverything(drawObj.drawing, matView, mainObj);
            clearTriVec(objMgr.vecTrianglesToRaster);
            editor.isDrawMode = false;
            editor.saveDrawing = false;
            editor.clearScreen = true;

        }

        if (editor.clearScreen) {
            clearTriVec(objMgr.vecTrianglesToRaster);
            // clearTriVec(drawObj.drawing);

        }

        // mainObj = true;
        handleMesh(mesh.tris, matView);


        handleDrawing(drawObj.drawing, matView);


        //Sort triangles from back to front
        std::sort(objMgr.vecTrianglesToRaster.begin(), objMgr.vecTrianglesToRaster.end(), [](Triangle& t1, Triangle& t2)
            {
                float z1 = (t1.t[0].z + t1.t[1].z + t1.t[2].z) / 3.0f;
                float z2 = (t2.t[0].z + t2.t[1].z + t2.t[2].z) / 3.0f;
                return z1 > z2;
            });
	}

    void handleDrawing(std::vector<Triangle>& drawing, mat4x4& matView) {

        for (auto& tri : drawing) {
            Triangle triTransformed, triViewed;

            math.Graphics_WorldMatrixTransform(triTransformed, matWorld, tri);

            vec3D normal;

            math.Graphics_CalcNormal(normal, triTransformed);

            vec3D vCameraRay = math.Vector_Sub(triTransformed.t[0], vCamera);

            if (math.Vector_DotProduct(normal, vCameraRay) < 0.0f) {
                math.Graphics_WorldSpacetoViewSpace(triViewed, triTransformed, matView);
                math.Graphics_3Dto2D(triDrawingProjected, matProj, triViewed);
                // drawObj.drawing.push_back(triDrawingProjected);
            }

        }
        
        // std::cout << "Triangle amount: #" << drawing.size() << std::endl;
    }

    void handleMesh(std::vector<Triangle>& meshTris, mat4x4& matView) {

        for (auto& tri : meshTris) {
            /*if (!mainObj)
                std::cout << "Triangle amount: #" << meshTris.size() << std::endl;*/

            Triangle triTransformed, triViewed;

            // World Matrix Transform
            /*triTransformed.t[0] = math.Matrix_MultiplyVector(matWorld, tri.t[0]);
            triTransformed.t[1] = math.Matrix_MultiplyVector(matWorld, tri.t[1]);
            triTransformed.t[2] = math.Matrix_MultiplyVector(matWorld, tri.t[2]);
            triTransformed.tc[0] = tri.tc[0];
            triTransformed.tc[1] = tri.tc[1];
            triTransformed.tc[2] = tri.tc[2];*/

            //World Matrix Transform
            math.Graphics_WorldMatrixTransform(triTransformed, matWorld, tri);

            // Calc triangle normal
            vec3D normal;

            math.Graphics_CalcNormal(normal, triTransformed);


            //line1 = math.Vector_Sub(triTransformed.t[1], triTransformed.t[0]);
            //line2 = math.Vector_Sub(triTransformed.t[2], triTransformed.t[0]);

            //normal = math.Vector_CrossProduct(line1, line2);

            //normal = math.Vector_Normalise(normal);

            //// normal = math.Vector_Mul(normal, -1);


            // Get ray from triangle to camera
            vec3D vCameraRay = math.Vector_Sub(triTransformed.t[0], vCamera);

            if (math.Vector_DotProduct(normal, vCameraRay) < 0.0f)
            {
                // Illumination
                vec3D lightDirection = { 0.0f, 1.0f, -1.0f };
                lightDirection = math.Vector_Normalise(lightDirection);
                lightDirection = math.Vector_Div(lightDirection, 1);

                // How "aligned" are light direction and triangle surface normal?
                float dp = std::max(0.1f, math.Vector_DotProduct(lightDirection, normal));

                vec3D vOffsetView = { 1,1,0 };

                //// Convert World Space --> View Space
                //triViewed.t[0] = math.Matrix_MultiplyVector(matView, triTransformed.t[0]);
                //triViewed.t[1] = math.Matrix_MultiplyVector(matView, triTransformed.t[1]);
                //triViewed.t[2] = math.Matrix_MultiplyVector(matView, triTransformed.t[2]);
                //triViewed.tc[0] = triTransformed.tc[0];
                //triViewed.tc[1] = triTransformed.tc[1];
                //triViewed.tc[2] = triTransformed.tc[2];

                math.Graphics_WorldSpacetoViewSpace(triViewed, triTransformed, matView);

                // Clip Viewed Triangle against near plane, this could form two additional triangles
                int nClippedTriangles = 0;
                Triangle clipped[2];
                nClippedTriangles = math.Triangle_ClipAgainstPlane(
                    { 0.0f, 0.0f, 0.1f }, { 0.0f, 0.0f, 1.f }, triViewed, clipped[0], clipped[1]);

                for (int n = 0; n < nClippedTriangles; n++)
                {

                    // Project triangles from 3D --> 2D
                    /*triProjected.t[0] = math.Matrix_MultiplyVector(matProj, clipped[n].t[0]);
                    triProjected.t[1] = math.Matrix_MultiplyVector(matProj, clipped[n].t[1]);
                    triProjected.t[2] = math.Matrix_MultiplyVector(matProj, clipped[n].t[2]);
                    triProjected.tc[0] = clipped[n].tc[0];
                    triProjected.tc[1] = clipped[n].tc[1];
                    triProjected.tc[2] = clipped[n].tc[2];*/

                    math.Graphics_3Dto2D(triProjected, matProj, triViewed);

                    // Scale into view
                    triProjected.t[0] = math.Vector_Div(triProjected.t[0], triProjected.t[0].w);
                    triProjected.t[1] = math.Vector_Div(triProjected.t[1], triProjected.t[1].w);
                    triProjected.t[2] = math.Vector_Div(triProjected.t[2], triProjected.t[2].w);

                    // X/Y are inverted so put them back
                    triProjected.t[0].x *= -1.0f;
                    triProjected.t[1].x *= -1.0f;
                    triProjected.t[2].x *= -1.0f;
                    triProjected.t[0].y *= -1.0f;
                    triProjected.t[1].y *= -1.0f;
                    triProjected.t[2].y *= -1.0f;

                    // Offset verts into visible normalised space
                    triProjected.t[0] = math.Vector_Add(triProjected.t[0], vOffsetView);
                    triProjected.t[1] = math.Vector_Add(triProjected.t[1], vOffsetView);
                    triProjected.t[2] = math.Vector_Add(triProjected.t[2], vOffsetView);
                    triProjected.t[0].x *= 0.5f * (float)ScreenDimensions.x;
                    triProjected.t[0].y *= 0.5f * (float)ScreenDimensions.y;
                    triProjected.t[1].x *= 0.5f * (float)ScreenDimensions.x;
                    triProjected.t[1].y *= 0.5f * (float)ScreenDimensions.y;
                    triProjected.t[2].x *= 0.5f * (float)ScreenDimensions.x;
                    triProjected.t[2].y *= 0.5f * (float)ScreenDimensions.y;

                    // Store triangle for sorting
                    /*if (mainObj)
                        objMgr.vecTrianglesToRaster.push_back(triProjected);
                    if (!mainObj)
                        drawObj.drawing.push_back(triProjected);*/

                    objMgr.vecTrianglesToRaster.push_back(triProjected);

                }
            }
        }

    }

    sf::Vector2f getScreenDimensions() {
        return ScreenDimensions;
    }

private:

    sf::Vector2f ScreenDimensions = sf::Vector2f(1200.f, 1000.f);


	void loadMesh(ObjectMgr& objMgr, Mesh& mesh) {
		objMgr.createCubeMesh(mesh);
	}

	void createProjectionMatrix(Math& math, mat4x4& matProj) {
		matProj = math.Matrix_MakeProjection(90.0f, ScreenDimensions.y / ScreenDimensions.x, 0.1f, 1000.f);
	}

    void TakeInputNewThread() {
        std::thread takeInput(&RunTimeEditor::takeInput, &editor);
        takeInput.detach();
    }

    
};