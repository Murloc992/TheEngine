// #include "Precomp.h"
#include "Camera.h"
#include "application/AppContext.h"
#include "application/Window.h"
#include "opengl/AABB.h"
#include "utility/Logger.h"

Camera::Camera(const glm::vec3 pos, const glm::vec3 target, const glm::vec3 up, const float aspect_ratio, const float field_of_view,
               const float near_z, const float far_z, const bool frustum_capable) {
  m_fps = true;
  m_hasFrustum = false;
  m_frustumCapable = frustum_capable;
  m_pos = pos;
  m_target = target;
  m_rot = glm::toQuat(glm::inverse(glm::lookAt(pos, target, up)));

  m_look = glm::normalize(pos - target);
  m_up = up;

  m_fov = field_of_view;
  m_far = far_z;
  m_near = near_z;
  m_aspect_ratio = aspect_ratio;
  m_translation = glm::vec3(0);
  m_last_mouse_pos = m_current_mouse_pos = glm::vec2(0);

  m_P = glm::perspective(glm::radians(field_of_view), aspect_ratio, near_z, far_z);
}

Camera::~Camera() {
  GetContext().GetWindow()->SetCursorDisabled(false);
}

void Camera::SetFPS(bool fps) {
  m_fps = fps;
  GetContext().GetWindow()->SetCursorDisabled(fps);
}

void Camera::ResetOrientation(glm::vec3 lookDir) {
  m_P = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);
}

void Camera::InitFrustum() {
  m_hasFrustum = true;
  auto proj = GetViewProjMat();
  glm::vec4 row0 = glm::row(proj, 0);
  glm::vec4 row1 = glm::row(proj, 1);
  glm::vec4 row2 = glm::row(proj, 2);
  glm::vec4 row3 = glm::row(proj, 3);

  frustumPlanes[(uint32_t)FRUSTUM_PLANES::FP_RIGHT].SetNormalsAndD(-row0 + row3);

  frustumPlanes[(uint32_t)FRUSTUM_PLANES::FP_LEFT].SetNormalsAndD(row3 + row0);

  frustumPlanes[(uint32_t)FRUSTUM_PLANES::FP_BOTTOM].SetNormalsAndD(row3 + row1);

  frustumPlanes[(uint32_t)FRUSTUM_PLANES::FP_TOP].SetNormalsAndD(-row1 + row3);

  frustumPlanes[(uint32_t)FRUSTUM_PLANES::FP_FAR].SetNormalsAndD(-row2 + row3);

  frustumPlanes[(uint32_t)FRUSTUM_PLANES::FP_NEAR].SetNormalsAndD(row3 + row2);

  float tang = glm::tan(glm::radians(m_fov) * 0.5f);

  float farmod = m_far / 1.f;

  nearHeight = m_near * tang;
  nearWidth = nearHeight * m_aspect_ratio;
  farHeight = farmod * tang;
  farWidth = farHeight * m_aspect_ratio;

  auto nearclip = m_pos + glm::normalize(m_look) * m_near;
  auto farclip = m_pos + glm::normalize(m_look) * farmod;

  nearTopLeft = nearclip + (m_up * nearHeight / 2.f) - (m_right * nearWidth / 2.f);
  nearTopRight = nearclip + (m_up * nearHeight / 2.f) + (m_right * nearWidth / 2.f);
  nearBottomLeft = nearclip - (m_up * nearHeight / 2.f) - (m_right * nearWidth / 2.f);
  nearBottomRight = nearclip - (m_up * nearHeight / 2.f) + (m_right * nearWidth / 2.f);

  farTopLeft = farclip + (m_up * farHeight / 2.f) - (m_right * farWidth / 2.f);
  farTopRight = farclip + (m_up * farHeight / 2.f) + (m_right * farWidth / 2.f);
  farBottomLeft = farclip - (m_up * farHeight / 2.f) - (m_right * farWidth / 2.f);
  farBottomRight = farclip - (m_up * farHeight / 2.f) + (m_right * farWidth / 2.f);

  frustumPlanes[(uint32_t)FRUSTUM_PLANES::FP_TOP].SetPoints(farTopRight, farTopLeft, nearTopLeft, nearTopRight);                 // top plane
  frustumPlanes[(uint32_t)FRUSTUM_PLANES::FP_BOTTOM].SetPoints(farBottomLeft, farBottomRight, nearBottomRight, nearBottomLeft);  // bottom plane
  frustumPlanes[(uint32_t)FRUSTUM_PLANES::FP_LEFT].SetPoints(farTopLeft, farBottomLeft, nearBottomLeft, nearTopLeft);            // left plane
  frustumPlanes[(uint32_t)FRUSTUM_PLANES::FP_RIGHT].SetPoints(farBottomRight, farTopRight, nearTopRight, nearBottomRight);       // right plane
  frustumPlanes[(uint32_t)FRUSTUM_PLANES::FP_FAR].SetPoints(farBottomRight, farBottomLeft, farTopLeft, farTopRight);             // far plane
  frustumPlanes[(uint32_t)FRUSTUM_PLANES::FP_NEAR].SetPoints(nearBottomLeft, nearBottomRight, nearTopRight, nearTopLeft);        // near plane
}

Plane3d* Camera::GetFrustumPlanes() {
  return frustumPlanes;
}

INTERSECT_RESULT Camera::PointInFrustum(const glm::vec3 point) {
  INTERSECT_RESULT res = INTERSECT_RESULT::IR_INSIDE;
  int out = 0;
  for (int32_t i = 0; i < 6; i++) {
    if (frustumPlanes[i].Distance(point, 0.f) <= 0)
      out++;
  }
  if (out > 0) {
    res = INTERSECT_RESULT::IR_OUTSIDE;
  }
  return res;
}

INTERSECT_RESULT Camera::SphereInFrustum(const glm::vec3 center, float radius) {
  INTERSECT_RESULT res = INTERSECT_RESULT::IR_INSIDE;

  int out = 0;
  for (int32_t i = 0; i < 6; i++) {
    if (frustumPlanes[i].Distance(center, radius) <= 0)
      return INTERSECT_RESULT::IR_OUTSIDE;
  }

  return res;
}

INTERSECT_RESULT Camera::BoxInFrustum(const AABB& box) {
  auto points = box.CalculatePoints();

  INTERSECT_RESULT res = INTERSECT_RESULT::IR_INSIDE;

  int in = 0;
  int out = 0;

  for (auto i = 0; i < 6; i++) {
    in = 0;
    out = 0;

    for (int j = 0; j < 8 && in == 0; j++) {
      if (frustumPlanes[i].Distance(points[j], 0.f) < 0)
        out++;
      else
        in++;
    }

    if (!in) {
      return INTERSECT_RESULT::IR_OUTSIDE;
    } else if (out) {
      res = INTERSECT_RESULT::IR_INTERSECT;
    }
  }

  return res;
}

glm::mat4 Camera::GetProjectionMat() {
  return m_P;
}

const float Camera::GetFOV() const {
  return m_fov;
}

void Camera::SetFOV(const float& fov) {
  m_fov = fov;
  m_P = glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);
}

const float Camera::GetFar() const {
  return m_far;
}

const float Camera::GetNear() const {
  return m_near;
}

const float Camera::GetAspectRatio() const {
  return m_aspect_ratio;
}

const glm::vec3 Camera::GetLook() const {
  return m_look;
}

const glm::vec3 Camera::GetDirection() const {
  return m_direction;
}

const glm::vec3 Camera::GetRight() const {
  return m_right;
}

const glm::vec3 Camera::GetUp() const {
  return m_up;
}

const glm::vec3 Camera::GetPosition() const {
  return m_pos;
}

const glm::quat Camera::GetRotation() const {
  return m_rot;
}

void Camera::SetPosition(glm::vec3 pos) {
  m_pos = pos;
}

const glm::vec3 Camera::GetTarget() const {
  return m_target;
}

void Camera::SetTarget(glm::vec3 target) {
  m_target = target;
}

void Camera::SetRotation(const glm::quat rotation) {
  m_rot = rotation;
}

void Camera::SetProjection(const glm::mat4 projection) {
  m_P = projection;
}

void Camera::Update(float dt) {
  m_pos += m_translation;
  m_translation = glm::vec3(0);

  if (m_fps) {
    auto window = GetContext().GetWindow();
    glm::ivec2 s = window->GetWindowSize();
    m_current_mouse_pos = window->GetMousePos();
    window->SetMousePos(s / 2);
    m_last_mouse_pos = window->GetMousePos();
    HandleMouse();
  }

  CalculateOrientationVectors();

  if (m_frustumCapable)
    InitFrustum();
}

void Camera::Walk(const float amount) {
  m_translation += (m_look * amount);
}

void Camera::Strafe(const float amount) {
  m_translation += (m_right * amount);
}

void Camera::Lift(const float amount) {
  m_translation += (m_up * amount);
}

glm::mat4x4 Camera::GetViewMat() {
  return glm::lookAt(m_pos, m_target, m_up);
}

glm::mat4x4 Camera::GetViewProjMat() {
  return GetProjectionMat() * GetViewMat();
}

void Camera::Orbit(glm::vec3 point, float distance, float verticalAngle, float horizontalAngle) {
  float camX = distance * sin(glm::radians(horizontalAngle)) * sin(glm::radians(verticalAngle));
  float camY = distance * cos(glm::radians(verticalAngle));
  float camZ = distance * cos(glm::radians(horizontalAngle)) * sin(glm::radians(verticalAngle));

  m_pos = point + glm::vec3(camX, camY, camZ);
  m_rot = glm::toQuat(glm::inverse(glm::lookAt(m_pos, point, glm::vec3(0, 1, 0))));
  CalculateOrientationVectors();
}

void Camera::HandleMouse() {
  glm::ivec2 delta_pos = m_current_mouse_pos - m_last_mouse_pos;

  if (glm::abs(delta_pos.x) > 64 || glm::abs(delta_pos.y) > 64)  /// probably some random shit happened, discard
    return;

  glm::quat r = m_rot;

  glm::quat rot_x(glm::vec3(0, -delta_pos.x / 100.0f, 0)), rot_y(glm::vec3(-delta_pos.y / 100.0f, 0, 0));

  r = rot_x * r;
  r = r * rot_y;

  m_rot = r;
}

void Camera::CalculateOrientationVectors() {
  m_look = glm::normalize(glm::vec3(m_rot * WORLD_FORWARD));
  m_up = glm::vec3(m_rot * WORLD_UP);
  m_right = glm::cross(m_look, m_up);
  m_target = m_pos + m_look;
  // direction is the reverse of look
  m_direction = glm::normalize(-1.f * m_look);
}
