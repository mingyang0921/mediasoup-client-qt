#pragma once

#include <memory>
#include <QObject>
#include "service/i_media_controller.h"
#include "service/i_participant_event_handler.h"
#include "api/media_stream_interface.h"

class ParticipantEventHandlerWrapper : public QObject, public vi::IParticipantEventHandler, public std::enable_shared_from_this<ParticipantEventHandlerWrapper>
{
    Q_OBJECT
public:
    ParticipantEventHandlerWrapper(QObject* parent = nullptr);

    void init();

    void destroy();

Q_SIGNALS:
    void participantJoin(std::shared_ptr<vi::IParticipant> participant);

    void participantLeave(std::shared_ptr<vi::IParticipant> participant);

    void remoteActiveSpeaker(std::shared_ptr<vi::IParticipant> participant, int32_t volume);

    void displayNameChanged(std::shared_ptr<vi::IParticipant> participant);

    void createRemoteVideoTrack(std::shared_ptr<vi::IParticipant> participant, const std::string& tid, rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> track);

    void removeRemoteVideoTrack(std::shared_ptr<vi::IParticipant> participant, const std::string& tid, rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> track);

    void remoteAudioStateChanged(std::shared_ptr<vi::IParticipant> participant, bool muted);

    void remoteVideoStateChanged(std::shared_ptr<vi::IParticipant> participant, bool muted);

protected:
    void onParticipantJoin(std::shared_ptr<vi::IParticipant> participant) override;

    void onParticipantLeave(std::shared_ptr<vi::IParticipant> participant) override;

    void onRemoteActiveSpeaker(std::shared_ptr<vi::IParticipant> participant, int32_t volume) override;

    void onDisplayNameChanged(std::shared_ptr<vi::IParticipant> participant) override;

    void onCreateRemoteVideoTrack(std::shared_ptr<vi::IParticipant> participant, const std::string& tid, rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> track) override;

    void onRemoveRemoteVideoTrack(std::shared_ptr<vi::IParticipant> participant, const std::string& tid, rtc::scoped_refptr<webrtc::MediaStreamTrackInterface> track) override;

    void onRemoteAudioStateChanged(std::shared_ptr<vi::IParticipant> participant, bool muted) override;

    void onRemoteVideoStateChanged(std::shared_ptr<vi::IParticipant> participant, bool muted) override;
};

