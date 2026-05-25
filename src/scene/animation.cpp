#include "scene/animation.hpp"
#include "core/object3d.hpp"
#include "math/Vec3.hpp"
#include "math/Quat.hpp"
#include "math/math_utils.hpp"
#include <algorithm>

namespace ygl {

// --- Keyframe ---
Keyframe::Keyframe()
    : time(0.0f),
      position(0.0f),
      rotation(0.0f, 0.0f, 0.0f, 1.0f),
      scale(1.0f) {
}

Keyframe::Keyframe(float time, const vec3& pos, const quat& rot, const vec3& scl)
    : time(time), position(pos), rotation(rot), scale(scl) {
}

// --- AnimationCurve ---
AnimationCurve::AnimationCurve() : loop(false), speed(1.0f) {
}

AnimationCurve::~AnimationCurve() {
    keyframes.clear();
}

void AnimationCurve::AddKeyframe(const Keyframe& keyframe) {
    keyframes.push_back(keyframe);
    std::sort(keyframes.begin(), keyframes.end(),
        [](const Keyframe& a, const Keyframe& b) { return a.time < b.time; });
}

void AnimationCurve::RemoveKeyframe(size_t index) {
    if (index < keyframes.size()) {
        keyframes.erase(keyframes.begin() + index);
    }
}

void AnimationCurve::ClearKeyframes() {
    keyframes.clear();
}

size_t AnimationCurve::GetKeyframeCount() const {
    return keyframes.size();
}

const Keyframe& AnimationCurve::GetKeyframe(size_t index) const {
    if (index < keyframes.size()) {
        return keyframes[index];
    }
    static Keyframe default_keyframe;
    return default_keyframe;
}

const std::vector<Keyframe>& AnimationCurve::GetKeyframes() const {
    return keyframes;
}

void AnimationCurve::SetLoop(bool l) {
    loop = l;
}

bool AnimationCurve::IsLooping() const {
    return loop;
}

void AnimationCurve::SetSpeed(float s) {
    speed = s;
}

float AnimationCurve::GetSpeed() const {
    return speed;
}

void AnimationCurve::GetCurrentKeyframes(float time, size_t& index1, size_t& index2, float& t) const {
    if (keyframes.empty()) {
        index1 = index2 = 0;
        t = 0.0f;
        return;
    }

    if (keyframes.size() == 1) {
        index1 = index2 = 0;
        t = 0.0f;
        return;
    }

    time *= speed;

    if (loop && !keyframes.empty()) {
        float max_time = keyframes.back().time;
        time = fmod(time, max_time);
    }

    for (size_t i = 0; i < keyframes.size() - 1; ++i) {
        if (time >= keyframes[i].time && time <= keyframes[i+1].time) {
            index1 = i;
            index2 = i + 1;
            float time_range = keyframes[index2].time - keyframes[index1].time;
            if (time_range > 0.0f) {
                t = (time - keyframes[index1].time) / time_range;
            } else {
                t = 0.0f;
            }
            return;
        }
    }

    if (loop) {
        index1 = keyframes.size() - 1;
        index2 = 0;
        float time_range = keyframes[0].time + (keyframes.back().time - keyframes[0].time);
        t = (time - keyframes.back().time) / time_range;
    } else {
        index1 = index2 = keyframes.size() - 1;
        t = 0.0f;
    }
}

void AnimationCurve::Evaluate(float time, vec3& position, quat& rotation, vec3& scale) const {
    size_t index1, index2;
    float t;
    GetCurrentKeyframes(time, index1, index2, t);

    const Keyframe& k1 = keyframes[index1];
    const Keyframe& k2 = keyframes[index2];

    position = lerp(k1.position, k2.position, t);
    scale = lerp(k1.scale, k2.scale, t);
    rotation = slerp(k1.rotation, k2.rotation, t);
}

// --- Animation ---
Animation::Animation()
    : duration(0.0f),
      current_time(0.0f),
      playing(false),
      loop(false),
      speed(1.0f) {
}

Animation::Animation(float dur)
    : duration(dur),
      current_time(0.0f),
      playing(false),
      loop(false),
      speed(1.0f) {
}

Animation::~Animation() {
    for (auto& curve : curves) {
        delete curve.second;
    }
    curves.clear();
    targets.clear();
}

void Animation::AddCurve(Object3D* target, AnimationCurve* curve) {
    if (target && curve) {
        curves[target] = curve;
        if (std::find(targets.begin(), targets.end(), target) == targets.end()) {
            targets.push_back(target);
        }

        if (!curve->GetKeyframes().empty()) {
            float curve_duration = curve->GetKeyframes().back().time;
            duration = std::max(duration, curve_duration);
        }
    }
}

void Animation::RemoveCurve(Object3D* target) {
    auto it = curves.find(target);
    if (it != curves.end()) {
        delete it->second;
        curves.erase(it);
        targets.erase(std::remove(targets.begin(), targets.end(), target), targets.end());
        RecalculateDuration();
    }
}

void Animation::RecalculateDuration() {
    duration = 0.0f;
    for (const auto& pair : curves) {
        if (!pair.second->GetKeyframes().empty()) {
            float curve_duration = pair.second->GetKeyframes().back().time;
            duration = std::max(duration, curve_duration);
        }
    }
}

void Animation::Play() {
    playing = true;
}

void Animation::Stop() {
    playing = false;
}

void Animation::Pause() {
    playing = false;
}

void Animation::Resume() {
    playing = true;
}

void Animation::Reset() {
    current_time = 0.0f;
}

void Animation::SetTime(float time) {
    current_time = time;
    if (current_time > duration) {
        if (loop) {
            current_time = fmod(current_time, duration);
        } else {
            current_time = duration;
        }
    }
}

float Animation::GetTime() const {
    return current_time;
}

float Animation::GetDuration() const {
    return duration;
}

void Animation::SetDuration(float dur) {
    duration = dur;
}

void Animation::SetLoop(bool l) {
    loop = l;
}

bool Animation::IsLooping() const {
    return loop;
}

void Animation::SetSpeed(float s) {
    speed = s;
}

float Animation::GetSpeed() const {
    return speed;
}

bool Animation::IsPlaying() const {
    return playing;
}

bool Animation::IsFinished() const {
    return !loop && current_time >= duration;
}

void Animation::SetName(const std::string& n) {
    name = n;
}

const std::string& Animation::GetName() const {
    return name;
}

const std::vector<Object3D*>& Animation::GetTargets() const {
    return targets;
}

AnimationCurve* Animation::GetCurve(Object3D* target) const {
    auto it = curves.find(target);
    if (it != curves.end()) {
        return it->second;
    }
    return nullptr;
}

void Animation::Update(float delta_time) {
    if (!playing) return;

    current_time += delta_time * speed;

    if (loop && current_time > duration) {
        current_time = fmod(current_time, duration);
    } else if (current_time > duration) {
        current_time = duration;
        if (!loop) {
            playing = false;
        }
    }

    for (const auto& pair : curves) {
        Object3D* target = pair.first;
        AnimationCurve* curve = pair.second;

        if (target && curve) {
            vec3 position, scale;
            quat rotation;
            curve->Evaluate(current_time, position, rotation, scale);

            target->SetPosition(position);
            target->SetRotation(rotation);
            target->SetScale(scale);
        }
    }
}

// --- Animation System ---
AnimationSystem::AnimationSystem() {
}

AnimationSystem::~AnimationSystem() {
    ClearAnimations();
}

void AnimationSystem::AddAnimation(Animation* animation) {
    if (animation) {
        animations.push_back(animation);
    }
}

void AnimationSystem::RemoveAnimation(Animation* animation) {
    auto it = std::find(animations.begin(), animations.end(), animation);
    if (it != animations.end()) {
        delete *it;
        animations.erase(it);
    }
}

void AnimationSystem::RemoveAnimation(size_t index) {
    if (index < animations.size()) {
        delete animations[index];
        animations.erase(animations.begin() + index);
    }
}

void AnimationSystem::ClearAnimations() {
    for (Animation* animation : animations) {
        delete animation;
    }
    animations.clear();
}

size_t AnimationSystem::GetAnimationCount() const {
    return animations.size();
}

Animation* AnimationSystem::GetAnimation(size_t index) const {
    return (index < animations.size()) ? animations[index] : nullptr;
}

Animation* AnimationSystem::GetAnimation(const std::string& name) const {
    for (Animation* animation : animations) {
        if (animation && animation->GetName() == name) {
            return animation;
        }
    }
    return nullptr;
}

void AnimationSystem::Update(float delta_time) {
    for (Animation* animation : animations) {
        if (animation && animation->IsPlaying()) {
            animation->Update(delta_time);
        }
    }
}

void AnimationSystem::PlayAll() {
    for (Animation* animation : animations) {
        if (animation) {
            animation->Play();
        }
    }
}

void AnimationSystem::StopAll() {
    for (Animation* animation : animations) {
        if (animation) {
            animation->Stop();
        }
    }
}

void AnimationSystem::ResetAll() {
    for (Animation* animation : animations) {
        if (animation) {
            animation->Reset();
        }
    }
}

} // namespace ygl
