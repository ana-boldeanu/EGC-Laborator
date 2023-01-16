#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace m1
{
    class Camera
    {
     public:
        Camera()
        {
            position    = glm::vec3(0, 0, 0);
            forward     = glm::vec3(0, 0, -1);
            up          = glm::vec3(0, 1, 0);
            right       = glm::vec3(1, 0, 0);
            distanceToTarget = 8;
        }

        Camera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            Set(position, center, up);
        }

        ~Camera()
        { }

        // Update camera
        void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
        {
            this->position = position;
            forward     = glm::normalize(center - position);
            right       = glm::cross(forward, up);
            this->up    = glm::cross(right, forward);
        }

        void MoveForward(float distance)
        {
            // Translate the camera using the direction vector computed from
            // `forward`. Movement will always keep the camera at the same height.
            glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
            position += dir * distance;
        }

        void TranslateForward(float distance)
        {
            // Translate the camera using the `forward` vector.
            position += glm::normalize(forward) * distance;
        }

        void TranslateUpward(float distance)
        {
            // Translate the camera using the `up` vector.
            position += glm::normalize(up) * distance;
        }

        void TranslateRight(float distance)
        {
            // You need to translate the camera using the `right` vector.
            // Usually, however, translation using camera's `right` vector
            // is not very useful, because if the camera is rotated around the
            // `forward` vector, then the translation on the `right` direction
            // will have an undesired effect, more precisely, the camera will
            // get closer or farther from the ground. The solution is to
            // actually use the projected `right` vector (projected onto the
            // ground plane), which makes more sense because we will keep the
            // same distance from the ground plane.
            position += glm::normalize(glm::vec3(right.x, 0, right.z)) * distance;
        }

        void RotateFirstPerson_OX(float angle)
        {
            // Compute the new `forward` and `up` vectors
            glm::mat4 model = glm::mat4(1.0f);
            forward = glm::normalize(glm::vec3(glm::rotate(model, angle, right) * glm::vec4(forward, 1.0)));

            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OY(float angle)
        {
            // Compute the new `forward`, `up` and `right`
            glm::mat4 model = glm::mat4(1.0f);
            forward = glm::normalize(glm::vec3(glm::rotate(model, angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1.0)));

            right = glm::normalize(glm::vec3(glm::rotate(model, angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1.0)));

            up = glm::cross(right, forward);
        }

        void RotateFirstPerson_OZ(float angle)
        {
            // Compute the new `right` and `up`
            glm::mat4 model = glm::mat4(1.0f);
            right = glm::normalize(glm::vec3(glm::rotate(model, angle, forward) * glm::vec4(right, 1.0)));

            up = glm::cross(right, forward);
        }

        void RotateThirdPerson_OX(float angle)
        {
            // Rotate the camera in third-person mode around the OX axis
            TranslateRight(distanceToTarget);
            RotateFirstPerson_OX(angle);
            TranslateRight(-distanceToTarget);
        }

        void RotateThirdPerson_OY(float angle)
        {
            // Rotate the camera in third-person mode around the OY axis
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OY(angle);
            TranslateForward(-distanceToTarget);
        }

        void RotateThirdPerson_OZ(float angle)
        {
            // Rotate the camera in third-person mode around the OZ axis
            TranslateForward(distanceToTarget);
            RotateFirstPerson_OZ(angle);
            TranslateForward(-distanceToTarget);
        }

        glm::mat4 GetViewMatrix()
        {
            // Returns the view matrix
            return glm::lookAt(position, position + forward, up);
        }

        glm::vec3 GetTargetPosition()
        {
            return position + forward * distanceToTarget;
        }

     public:
        float distanceToTarget;
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 right;
        glm::vec3 up;
    };
}