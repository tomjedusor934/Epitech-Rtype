/*
** EPITECH PROJECT, 2023
** ecs_rType
** File description:
** buttonmenu
*/

#pragma once
#include "interface.hpp"

namespace rtype::ECS::Ecs3D {
    class ObjectTransform : public IComponent {
        private:
            Vector3 Position = { 0 };
            Quaternion Orientation = QuaternionIdentity();

            bool Dirty = true;

            Matrix WorldMatrix = { 0 };
            Matrix GlWorldMatrix = { 0 };

            ObjectTransform* Parent = nullptr;
            std::vector<ObjectTransform*> Children;

        public:

            ObjectTransform(bool faceY = true)
            {
                if (faceY)
                    Orientation = QuaternionFromAxisAngle(Vector3{0,1,0}, 0);
            }

            inline ObjectTransform* GetParent() const { return Parent; }

            inline const std::vector<ObjectTransform*>& GetChildren() const { return Children; }

            inline ObjectTransform* AddChild(ObjectTransform* child)
            {
                if (!child)
                    return nullptr;

                child->Reparent(this);

                return child;
            }

            inline ObjectTransform& AddChild()
            {
                ObjectTransform* child = new ObjectTransform();
                child->Reparent(this);

                return *child;
            }

            inline ObjectTransform& AddChild(ObjectTransform& child)
            {
                child.Reparent(this);

                return child;
            }

            inline void Reparent(ObjectTransform* newParent)
            {
                if (Parent == newParent)
                    return;

                if (newParent)
                {
                    auto child = std::find(newParent->Children.begin(), newParent->Children.end(), this);
                    if (child != newParent->Children.end())
                        newParent->Children.erase(child);
                }

                Parent = newParent;
                if (Parent)
                    Parent->Children.push_back(this);
            }

            inline void Detach()
            {
                if (!GetParent())
                    return;

                Matrix worldTransform = GetWorldMatrix();
                Position = Vector3Transform(Vector3Zero(), WorldMatrix);

                Matrix translateMatrix = MatrixTranslate(Position.x, Position.y, Position.z);
                Matrix orientationMatrix = MatrixMultiply(worldTransform, translateMatrix);

                Orientation = QuaternionFromMatrix(WorldMatrix);

                Reparent(nullptr);
            }

            inline void SetDirty()
            {
                Dirty = true;
                for (ObjectTransform* childTransform : Children)
                {
                    if (childTransform != nullptr)
                        childTransform->SetDirty();
                }
            }

            inline const Vector3& GetPosition() const { return Position; }

            inline Quaternion GetOrientation()
            {
                return Orientation;
            }

            inline Vector3 GetEulerAngles()
            {
                return QuaternionToEuler(Orientation);
            }

            inline Vector3 GetDVector() const
            {
                return Vector3Transform(Vector3{ 0, 0, 1 }, MatrixInvert(QuaternionToMatrix(Orientation)));
            }

            inline Vector3 GeVVector() const
            {
                return Vector3Transform(Vector3{ 0, 1, 0 }, MatrixInvert(QuaternionToMatrix(Orientation)));
            }

            inline Vector3 GetHNegVector()
            {
                return Vector3CrossProduct(GeVVector(), GetDVector());
            }

            inline Vector3 GetHPostVector()
            {
                return Vector3CrossProduct(GetDVector(), GeVVector());
            }

            inline Vector3 GetWorldPosition()
            {
                Matrix worldTransform = GetWorldMatrix();
                return Vector3Transform(Vector3Zero(), WorldMatrix);
            }

            inline Vector3 GetWorldTarget()
            {
                Matrix worldTransform = GetWorldMatrix();
                Vector3 pos = Vector3Transform(Vector3Zero(), WorldMatrix);

                Matrix translateMatrix = MatrixTranslate(Position.x, Position.y, Position.z);
                Matrix orientationMatrix = MatrixMultiply(worldTransform, translateMatrix);

                return Vector3Add(pos, Vector3Transform(Vector3{ 0 , 1 , 0 }, WorldMatrix));
            }

            inline void SetPosition(float x, float y, float z)
            {
                Position.x = x;
                Position.y = y;
                Position.z = z;
                SetDirty();
            }

            inline void SetPosition(const Vector3& pos)
            {
                Position = pos;
                SetDirty();
            }

            inline void SetPositionX(float x)
            {
                Position.x = x;
                SetDirty();
            }

            inline void SetPositionY(float y)
            {
                Position.y = y;
                SetDirty();
            }

            inline void SetPositionZ(float z)
            {
                Position.z = z;
                SetDirty();
            }

            inline void SetOrientation(const Vector3& eulerAngles)
            {
                Vector3 angles = Vector3Scale(eulerAngles, DEG2RAD);
                Orientation = QuaternionFromEuler(angles.x, angles.y, angles.z);
                SetDirty();
            }

            inline void SetOrientation(const Quaternion& orientation)
            {
                Orientation = orientation;
                SetDirty();
            }

            inline bool IsDirty()
            {
                return Dirty;
            }

            inline void LookAt(const Vector3& target, const Vector3& up)
            {
                SetDirty();
                Matrix mat = MatrixLookAt(Position, target, up);
                Orientation = QuaternionFromMatrix(mat);
            }

            inline Matrix GetLocalMatrix()
            {
                Matrix orient = QuaternionToMatrix(Orientation);
                Matrix translation = MatrixTranslate(Position.x, Position.y, Position.z);

                return MatrixMultiply(MatrixInvert(orient), translation);
            }

            inline void UpdateWorldMatrix()
            {
                Matrix parentMatrix = MatrixIdentity();

                if (Parent)
                    parentMatrix = Parent->GetWorldMatrix();

                WorldMatrix = MatrixMultiply(GetLocalMatrix(), parentMatrix);
                GlWorldMatrix = MatrixTranspose(WorldMatrix);

                Dirty = false;
            }
            
            inline const Matrix& GetWorldMatrix()
            {
                if (!IsDirty())
                    return WorldMatrix;

                UpdateWorldMatrix();
                return WorldMatrix;
            }

            inline const Matrix& GetGLWorldMatrix()
            {
                if (!IsDirty())
                    return GlWorldMatrix;

                UpdateWorldMatrix();
                return GlWorldMatrix;
            }

            inline Vector3 ToLocalPos(const Vector3& inPos)
            {
                return Vector3Transform(inPos, MatrixInvert(GetWorldMatrix()));
            }


            // move along the vector (up / down) : positive vector = up, negative vector = down
            inline void MoveV(float distance)
            {
                SetDirty();
                Position = Vector3Add(Position, Vector3Scale(GeVVector(), distance));
            }

            // move along the vector (forward / backward) : positive vector = forward, negative vector = backward
            inline void MoveD(float distance)
            {
                SetDirty();
                Position = Vector3Add(Position, Vector3Scale(GetDVector(), distance));
            }

            // move along the vector (left / right) : positive vector = right, negative vector = left
            inline void MoveH(float distance)
            {
                SetDirty();
                Position = Vector3Add(Position, Vector3Scale(GetHNegVector(), distance));
            }

            inline void RotateV(float angle)
            {
                SetDirty();
                auto rot = QuaternionFromEuler(0, -angle * DEG2RAD, 0);
                Orientation = QuaternionMultiply(Orientation, rot);
            }

            inline void RotateH(float angle)
            {
                SetDirty();
                auto rot = QuaternionFromEuler(angle * DEG2RAD, 0, 0);
                Orientation = QuaternionMultiply(Orientation, rot);
            }

            inline void RotateD(float angle)
            {
                SetDirty();
                auto rot = QuaternionFromEuler(0, 0, -angle * DEG2RAD);
                Orientation = QuaternionMultiply(Orientation, rot);
            }

            inline void RotateX(float angle)
            {
                SetDirty();
                auto rot = QuaternionFromEuler(angle * DEG2RAD, 0, 0);
                Orientation = QuaternionMultiply(rot, Orientation);
            }

            inline void RotateY(float angle)
            {
                SetDirty();
                auto rot = QuaternionFromEuler(0, -angle * DEG2RAD, 0);
                Orientation = QuaternionMultiply(rot, Orientation);
            }

            inline void RotateZ(float angle)
            {
                SetDirty();
                auto rot = QuaternionFromEuler(0, 0, -angle * DEG2RAD);
                Orientation = QuaternionMultiply(rot, Orientation);
            }

            inline void PushMatrix()
            {
                const Matrix& glMatrix = GetGLWorldMatrix();
                rlPushMatrix();
                rlMultMatrixf((float*)(&glMatrix.m0));
            }

            inline void PopMatrix()
            {
                rlPopMatrix();
            }

    };
}