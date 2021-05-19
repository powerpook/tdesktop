/*
This file is part of Telegram Desktop,
the official desktop application for the Telegram messaging service.

For license and copyright information please follow this link:
https://github.com/telegramdesktop/tdesktop/blob/master/LEGAL
*/
#pragma once

#include "ui/rp_widget.h"

namespace style {
struct GroupCallLargeVideo;
} // namespace style

namespace Webrtc {
class VideoTrack;
} // namespace Webrtc

namespace Ui {
class AbstractButton;
class RpWidgetWrap;
namespace GL {
struct Capabilities;
struct ChosenRenderer;
} // namespace GL
} // namespace Ui

namespace Calls::Group {

class MembersRow;
enum class VideoQuality;

struct LargeVideoTrack {
	Webrtc::VideoTrack *track = nullptr;
	MembersRow *row = nullptr;

	[[nodiscard]] explicit operator bool() const {
		return track != nullptr;
	}
};

[[nodiscard]] inline bool operator==(
		LargeVideoTrack a,
		LargeVideoTrack b) noexcept {
	return (a.track == b.track) && (a.row == b.row);
}

[[nodiscard]] inline bool operator!=(
		LargeVideoTrack a,
		LargeVideoTrack b) noexcept {
	return !(a == b);
}

class LargeVideo final {
public:
	LargeVideo(
		QWidget *parent,
		const style::GroupCallLargeVideo &st,
		bool visible,
		rpl::producer<LargeVideoTrack> track,
		rpl::producer<bool> pinned);
	~LargeVideo();

	void raise();
	void setVisible(bool visible);
	void setGeometry(int x, int y, int width, int height);
	void setControlsShown(float64 shown);

	[[nodiscard]] rpl::producer<bool> pinToggled() const;
	[[nodiscard]] rpl::producer<> clicks() const {
		return _clicks.events();
	}

	[[nodiscard]] QSize trackSize() const;
	[[nodiscard]] rpl::producer<QSize> trackSizeValue() const;
	[[nodiscard]] rpl::producer<VideoQuality> requestedQuality() const;

	[[nodiscard]] rpl::lifetime &lifetime();

private:
	struct PinButton;

	[[nodiscard]] not_null<QWidget*> widget() const;

	void setup(
		rpl::producer<LargeVideoTrack> track,
		rpl::producer<bool> pinned);
	void setupControls(rpl::producer<bool> pinned);
	void paint(QRect clip, bool opengl);
	void paintControls(Painter &p, QRect clip);
	void updateControlsGeometry();
	void togglePinShown(bool shown);

	[[nodiscard]] Ui::GL::ChosenRenderer chooseRenderer(
		Ui::GL::Capabilities capabilities);

	const std::unique_ptr<Ui::RpWidgetWrap> _content;
	const style::GroupCallLargeVideo &_st;
	LargeVideoTrack _track;
	QImage _shadow;
	std::unique_ptr<PinButton> _pinButton;
	rpl::event_stream<> _clicks;
	const bool _smallLayout = true;
	bool _pinned = false;
	bool _mouseDown = false;
	float64 _controlsShownRatio = 1.;
	rpl::variable<QSize> _trackSize;
	rpl::variable<VideoQuality> _requestedQuality;
	rpl::lifetime _trackLifetime;

};

[[nodiscard]] QImage GenerateShadow(
	int height,
	int topAlpha,
	int bottomAlpha);

} // namespace Calls::Group