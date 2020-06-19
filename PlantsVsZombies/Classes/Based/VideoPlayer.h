#pragma
#if MYRELEASE
#include "Based/vlc/vlc.h"
#include "cocos2d.h"

#pragma comment (lib, "libvlc.lib")
#pragma comment (lib, "libvlccore.lib")

using namespace cocos2d;

class VideoPlayer : public Sprite
{
public:
    ~VideoPlayer();

    static VideoPlayer* instance(void);

    bool init(void);
    void play(const char* path);
    void play();
    void stop(void);
    void pause(void);
    void draw(Renderer* renderer, const Mat4& transform, uint32_t flags);
    void changeVideoState();
    void setPrecent(const float precent);
    float getPrecent();
    void setVolume(int volume);
    int getVolume() const;
    void setVideoTime(const libvlc_time_t time);
    libvlc_time_t getVideoTime() const;

protected:
    VideoPlayer();
  
private:
    libvlc_instance_t* vlc;
    libvlc_media_player_t* vlc_player;
    libvlc_media_list_player_t* vlc_list_player;
    libvlc_media_list_t* vlc_list;
    libvlc_media_t* vlc_media;
    libvlc_time_t video_length;

    unsigned int width;
    unsigned int height;

    static VideoPlayer* _instance;
};
#endif