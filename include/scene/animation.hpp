#pragma once

#include "math/Vec3.hpp"
#include "math/Quat.hpp"
#include <vector>
#include <string>
#include <memory>
#include <map>

namespace ygl {

class Object3D;

class Animation {
public:
    struct Keyframe {
        float time;
        Vec3 position;
        Quat rotation;
        Vec3 scale;
    };

    struct Channel {
        std::string nodeName;
        std::vector<Keyframe> keyframes;
        int interpolationType; // 0 = LINEAR, 1 = STEP, 2 = CUBIC
    };

    Animation(const std::string& name = "", float duration = 0.0f, float ticksPerSecond = 0.0f);
    ~Animation();

    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

    float getDuration() const { return m_duration; }
    void setDuration(float duration) { m_duration = duration; }

    float getTicksPerSecond() const { return m_ticksPerSecond; }
    void setTicksPerSecond(float tps) { m_ticksPerSecond = tps; }

    void addChannel(const Channel& channel);
    void removeChannel(const std::string& nodeName);
    Channel* getChannel(const std::string& nodeName);
    const Channel* getChannel(const std::string& nodeName) const;
    size_t getChannelCount() const { return m_channels.size(); }

    void update(float deltaTime);
    void applyTo(Object3D& object, float time) const;

    void reset();
    void play();
    void pause();
    void stop();
    bool isPlaying() const { return m_playing; }
    bool isPaused() const { return m_paused; }

    float getCurrentTime() const { return m_currentTime; }
    void setCurrentTime(float time);

private:
    std::string m_name;
    float m_duration;
    float m_ticksPerSecond;
    std::map<std::string, Channel> m_channels;

    float m_currentTime = 0.0f;
    bool m_playing = false;
    bool m_paused = false;

    Vec3 interpolatePosition(const Channel& channel, float time) const;
    Quat interpolateRotation(const Channel& channel, float time) const;
    Vec3 interpolateScale(const Channel& channel, float time) const;
};

class Animator {
public:
    Animator();
    ~Animator();

    void addAnimation(std::shared_ptr<Animation> animation);
    void removeAnimation(const std::string& name);
    void removeAnimation(std::shared_ptr<Animation> animation);
    std::shared_ptr<Animation> getAnimation(const std::string& name) const;
    size_t getAnimationCount() const { return m_animations.size(); }

    void setCurrentAnimation(const std::string& name);
    void setCurrentAnimation(std::shared_ptr<Animation> animation);
    std::shared_ptr<Animation> getCurrentAnimation() const { return m_currentAnimation; }

    void update(float deltaTime);
    void applyTo(Object3D& object) const;

    void play();
    void pause();
    void stop();
    bool isPlaying() const;
    float getCurrentTime() const;
    void setCurrentTime(float time);

private:
    std::map<std::string, std::shared_ptr<Animation>> m_animations;
    std::shared_ptr<Animation> m_currentAnimation;
    float m_blendFactor = 0.0f;
    bool m_playing = false;
};

} // namespace ygl
