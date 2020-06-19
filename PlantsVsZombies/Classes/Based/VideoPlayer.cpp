#if MYRELEASE
#include "VideoPlayer.h"
#include "base/CCDirector.h"

VideoPlayer* VideoPlayer::_instance = 0;
static char* videobuf = 0;

static void* lock(void* data, void** p_pixels)
{
    *p_pixels = videobuf;
    return NULL;
}

static void unlock(void* data, void* id, void* const* p_pixels)
{
    assert(id == NULL);
}

static void display(void* data, void* id)
{
    (void)data;
    assert(id == NULL);
}

VideoPlayer::VideoPlayer() :
    vlc(0), vlc_player(0), video_length(-1)
{
    init();
}

VideoPlayer::~VideoPlayer()
{
    CCSprite::~CCSprite();
    free(videobuf);

    libvlc_media_player_stop(vlc_player);
    libvlc_media_player_release(vlc_player);
    libvlc_release(vlc);
}

bool VideoPlayer::init(void)
{
    vlc = libvlc_new(0, nullptr);
    vlc_player = libvlc_media_player_new(vlc);

    Size size = Director::getInstance()->getWinSize();
    width = size.width;
    height = size.height;
    videobuf = (char*)malloc((width * height) << 2);
    memset(videobuf, 0, (width * height) << 2);
    libvlc_video_set_callbacks(vlc_player, lock, unlock, display, nullptr);
    libvlc_video_set_format(vlc_player, "RGBA", width, height, width << 2);

    Texture2D* texture = new Texture2D();
    texture->initWithData(videobuf, sizeof(videobuf), Texture2D::PixelFormat::RGBA8888, width, height, size);
    return initWithTexture(texture);
}

void VideoPlayer::play(const char* path)
{
    vlc_list = libvlc_media_list_new(vlc);
    vlc_media = libvlc_media_new_path(vlc, path);
    libvlc_media_list_add_media(vlc_list, vlc_media);

    vlc_list_player = libvlc_media_list_player_new(vlc);
    libvlc_media_list_player_set_media_list(vlc_list_player, vlc_list);
    libvlc_media_list_player_set_media_player(vlc_list_player, vlc_player);
    libvlc_media_list_player_set_playback_mode(vlc_list_player, libvlc_playback_mode_loop);
    libvlc_media_list_player_play(vlc_list_player);
}

void VideoPlayer::play()
{
    libvlc_media_player_play(vlc_player);
}

void VideoPlayer::stop(void)
{
    libvlc_media_player_stop(vlc_player);
}

void VideoPlayer::pause(void)
{
    libvlc_media_player_pause(vlc_player);
}

void VideoPlayer::draw(Renderer* renderer, const Mat4& transform, uint32_t flags)
{
    CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");

    CC_NODE_DRAW_SETUP();

    GL::blendFunc(_blendFunc.src, _blendFunc.dst);

    if (_texture != NULL)
    {
        GL::bindTexture2D(_texture->getName());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (uint8_t*)videobuf);
    }
    else
    {
        GL::bindTexture2D(static_cast<GLuint>(0));
    }

    //
    // Attributes
    //

    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POS_COLOR_TEX);

#define kQuadSize sizeof(_quad.bl)
    long offset = (long)&_quad;

    // vertex
    int diff = offsetof(ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_POSITION, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

    // texCoods
    diff = offsetof(ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_TEX_COORD, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));

    // color
    diff = offsetof(ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(GLProgram::VERTEX_ATTRIB_COLOR, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    CHECK_GL_ERROR_DEBUG();

    CC_INCREMENT_GL_DRAWS(1);

    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
}

void VideoPlayer::changeVideoState()
{
    switch (libvlc_media_get_state(vlc_media))
    {
    case libvlc_state_t::libvlc_Playing: pause(); break;
    case libvlc_state_t::libvlc_Paused:  play();  break;
    default: break;
    }
}

void VideoPlayer::setVideoTime(const libvlc_time_t time)
{
    libvlc_media_player_set_time(vlc_player, time);
}

libvlc_time_t VideoPlayer::getVideoTime() const
{
    return libvlc_media_player_get_time(vlc_player);
}

void VideoPlayer::setPrecent(const float precent)
{
    video_length < 0 ? video_length = libvlc_media_get_duration(vlc_media) : video_length;
    libvlc_media_player_set_time(vlc_player, static_cast<libvlc_time_t>(video_length * precent));
}

float VideoPlayer::getPrecent()
{
    auto videoCurrentTime = libvlc_media_player_get_time(vlc_player);
    video_length < 0 ? video_length = libvlc_media_get_duration(vlc_media) : video_length;
   
    if (videoCurrentTime != -1 && video_length != -1)
    {
        return videoCurrentTime * 1.0f / video_length * 100;
    }
    return -1;
}

void VideoPlayer::setVolume(int volume)
{
    if (volume < 0) volume = 0;
    if (volume > 100)volume = 100;
    
    libvlc_audio_set_volume(vlc_player, volume);
}

int VideoPlayer::getVolume() const
{
    return libvlc_audio_get_volume(vlc_player);
}

VideoPlayer* VideoPlayer::instance()
{
    if (_instance == 0)
        _instance = new VideoPlayer();
    return _instance;
}

#endif