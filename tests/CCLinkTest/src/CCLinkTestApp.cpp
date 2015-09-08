#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/Log.h"

#include "link/ccSerialLink.h"

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace soso;

class CCLinkTestApp : public App {
public:
	void setup() override;
	void update() override;
	void draw() override;
private:
	std::shared_ptr<soso::ccSerialLink> _serialLink;
	ci::gl::TextureFontRef							_font;

	// Number of chars to display onscreen
	int																	_maxCharBufferSize = 400;
	// Store chars to display onscreen
	std::deque<char>										_charBuffer;

	std::unique_ptr<ofstream>						_output_stream;
};

void CCLinkTestApp::setup()
{
	_output_stream = make_unique<ofstream>((getAssetPath("") / "closed_captioning.txt").string());

	_font = gl::TextureFont::create(Font("Arial", 14));
	_serialLink = std::make_shared<ccSerialLink>( io_service(), "/dev/tty.usbserial-AL00APKE", 9600 );


	_serialLink->addNewCharHandler( [this] (char newChar) {
		*_output_stream << newChar;
		cout << newChar;
	});

}

void CCLinkTestApp::update()
{
	_serialLink->update();
}

void CCLinkTestApp::draw()
{
	gl::clear( Color( 0, 0, 0 ) ); 
}

CINDER_APP( CCLinkTestApp, RendererGl )
