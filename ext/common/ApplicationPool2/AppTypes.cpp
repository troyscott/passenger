/*
 *  Phusion Passenger - https://www.phusionpassenger.com/
 *  Copyright (c) 2013 Phusion
 *
 *  "Phusion Passenger" is a trademark of Hongli Lai & Ninh Bui.
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 */
#include <ApplicationPool2/AppTypes.h>

namespace Passenger {
namespace ApplicationPool2 {

// Don't forget to update ApplicationPool2::Options::getStartCommand() too.
const AppTypeDefinition appTypeDefinitions[] = {
	{ PAT_RACK, "rack", "config.ru", "Passenger RackApp" },
	{ PAT_WSGI, "wsgi", "passenger_wsgi.py", "Passenger WsgiApp" },
	{ PAT_CLASSIC_RAILS, "classic-rails", "config/environment.rb", "Passenger ClassicRailsApp" },
	{ PAT_NODE, "node", "passenger_node.js", "Passenger NodeApp" },
	{ PAT_METEOR, "meteor", ".meteor", "Passenger MeteorApp" },
	{ PAT_NONE, NULL, NULL, NULL }
};

} // namespace ApplicationPool2
} // namespace Passenger


using namespace Passenger;
using namespace Passenger::ApplicationPool2;

PP_AppTypeDetector *
pp_app_type_detector_new() {
	return new AppTypeDetector();
}

void
pp_app_type_detector_free(PP_AppTypeDetector *detector) {
	delete (AppTypeDetector *) detector;
}

PassengerAppType
pp_app_type_detector_check_document_root(PP_AppTypeDetector *_detector,
	const char *documentRoot, unsigned int len, int resolveFirstSymlink)
{
	AppTypeDetector *detector = (AppTypeDetector *) _detector;
	return detector->checkDocumentRoot(StaticString(documentRoot, len), resolveFirstSymlink);
}

PassengerAppType
pp_app_type_detector_check_app_root(PP_AppTypeDetector *_detector,
	const char *appRoot, unsigned int len)
{
	AppTypeDetector *detector = (AppTypeDetector *) _detector;
	return detector->checkAppRoot(StaticString(appRoot, len));
}

const char *
pp_get_app_type_name(PassengerAppType type) {
	return getAppTypeName(type);
}
