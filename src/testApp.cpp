#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup()
{
    ofEnableAntiAliasing();
    //  set Video Size
    vidSize = ofVec2f(640, 480);
    ofSetFrameRate(30);
    //  initialize mouse point
    mouse_point = ofPoint(0,0);
    //  intialize video ID
    vidID = 0;
    //  initialize next Bot ID
    potential_bot_id = 0;
    //  initialized bot color number
    botColorNum = 0;
    //  set time stamp
    timeStamp = ofGetUnixTime();
    
    //  set up message queue thread
//    zmqThread = ZMQThreadedObject("5555");
    zmqThread.start();
    
	#ifdef _USE_LIVE_VIDEO

        vidGrabber.setVerbose(true);
        vidGrabber.initGrabber(vidSize.x,vidSize.y);
        colorImg.allocate(vidSize.x,vidSize.y);
        grayImage.allocate(vidSize.x,vidSize.y);
        grayBg.allocate(vidSize.x,vidSize.y);
        grayDiff.allocate(vidSize.x,vidSize.y);
        ofSetWindowShape(vidSize.x,vidSize.y);


	#else
        vidPlayer.loadMovie("../GridDetectionNoText.mov");
        vidPlayer.play();
        colorImg.allocate(vidPlayer.width,vidPlayer.height);
        grayImage.allocate(vidPlayer.width,vidPlayer.height);
        grayBg.allocate(vidPlayer.width,vidPlayer.height);
        grayDiff.allocate(vidPlayer.width,vidPlayer.height);
        ofSetWindowShape(vidPlayer.width,vidPlayer.height);

	#endif

    //  add many colors to vector for random assignment later
    colors[0] = ofColor::green;
    colors[1] =ofColor::aqua;
    colors[2] =ofColor::maroon;
    colors[3] =ofColor::lightCoral;

    colors[4] =ofColor::cyan;
    colors[5] =ofColor::red;
    colors[6] =ofColor::lightBlue;
    colors[7] =ofColor::salmon;
    colors[8] =ofColor::blueViolet;
    colors[9] =ofColor::hotPink;
    colors[10] =ofColor::honeyDew;
    colors[11] =ofColor::yellow;
    colors[12] =ofColor::limeGreen;
    colors[13] =ofColor::orange;
    colors[14] =ofColor::indigo;
    colors[15] =ofColor::fuchsia;
    colors[16] =ofColor::crimson;
    colors[17] =ofColor::purple;


	bLearnBakground = true;

    // GUI setup
    gui.setup();
    gui.add(alterationsLabel.setup("Alternations\n", ""));
    gui.add(thresholdSlider.setup("Threshold", 45, 0, 255));
    gui.add(dilateSlider.setup("Dilation", 4, 0, 20));
    gui.add(autoFindBots.setup("Auto Find", false));

    gui.add(infoLabel.setup("Displayed Information\n", ""));
    gui.add(drawReportStringToggle.setup("Info Table", true));
    gui.add(drawBotRectToggle.setup("Rectangles", false));
    gui.add(drawBotIDToggle.setup("ID", false));
    gui.add(drawBotPathToggle.setup("Path", false));
    gui.add(drawAssociationToggle.setup("Association", true));

    gui.add(imageLabel.setup("Image\n", ""));
    gui.add(showColorImage.setup("Show Color"));
    gui.add(showGrayImage.setup("Show Gray"));
    gui.add(showSubtractedImage.setup("Show Subtracted Image"));
    gui.add(showBackgroundImage.setup("Show Background"));

    showColorImage.addListener(this, &testApp::showColorPressed);
    showGrayImage.addListener(this, &testApp::showGrayPressed);
    showBackgroundImage.addListener(this, &testApp::showBGPressed);
    showSubtractedImage.addListener(this, &testApp::showSubtractedPressed);

    showGUI = false;
}

//--------------------------------------------------------------
void testApp::update()
{
    bool bNewFrame = false;

	#ifdef _USE_LIVE_VIDEO
       vidGrabber.update();
	   bNewFrame = vidGrabber.isFrameNew();
    #else
        vidPlayer.update();
        bNewFrame = vidPlayer.isFrameNew();
	#endif

	if (bNewFrame)
    {

		#ifdef _USE_LIVE_VIDEO
            colorImg.setFromPixels(vidGrabber.getPixels(), vidSize.x, vidSize.y);
	    #else
            colorImg.setFromPixels(vidPlayer.getPixels(), vidPlayer.width,vidPlayer.height);
        #endif

        grayImage = colorImg;
		if (bLearnBakground == true)
        {
			grayBg = grayImage;		// the = sign copys the pixels from grayImage into grayBg
            bots.clear();
			bLearnBakground = false;
		}

		// take the abs value of the difference between background and incoming and then threshold:
		grayDiff.absDiff(grayBg, grayImage);
		grayDiff.threshold(thresholdSlider);

        //  dilate the blobs on the screen; compensating for blob-splitting by lines on grid
        for (int i = 0; i < dilateSlider; i++)
            grayDiff.dilate();

		// find contours which are between the size of 1/300 pixels and 1/3 the w*h pixels.
		contourFinder.findContours(grayDiff,
                                   (colorImg.height * colorImg.width)/300,
                                   (colorImg.height * colorImg.width)/3,
                                   10,
                                   false);	// find bots

        //  get all the blobs found in the contourFinder
        blobs = vector<ofxCvBlob>(contourFinder.blobs.begin(), contourFinder.blobs.end());
        
        line = ofPolyline();
        for (map<int, Bot>::iterator it = bots.begin(); it != bots.end(); it++)
        {
            line.addVertex(it->second.getCenter());
        }
        
        
        //  if the contourFinder didn't find any blobs, remove all the bots stored.
        if (blobs.size() == 0)
        {
            bots.clear();
            potential_bot_id = 0;
            botColorNum = 0;
        } else
        {
            unsigned int time = ofGetUnixTime();
            timeStamp = time;
            //  vector iterator that will contain the pointer to the beginning of blobs
            vector<ofxCvBlob>::iterator vect_it;
            //  remove any elements that may resise in black_list
            black_list.clear();
            //  updated the positions of the blobs
            for (map<int, Bot>::iterator it = bots.begin(); it != bots.end(); it++)
            {
                //  this will contain the offset of one pointer to the pointer of first element
                offset = it->second.updatePosition(blobs);
                vect_it = blobs.begin();
                std::advance(vect_it, offset);
                if (offset != -1)
                {
                    blobs.erase(vect_it);
                } else
                {
                    //  push key to bot that no longer exist
                    black_list.push_back(it->first);
                }
            }

            //  if there are any keys in black_list, remove them from bots map
            for (vector<int>::iterator it = black_list.begin(); it != black_list.end(); it++)
            {
                bots.erase(*it);
            }
            
            if (autoFindBots)
            {
                //  insert newly-found blobs, if there are any
                for (vector<ofxCvBlob>::iterator it = blobs.begin(); it != blobs.end(); it++){
                    bots.insert(pair<int, Bot>(potential_bot_id, Bot(*it, potential_bot_id, colors[botColorNum])));
                    botColorNum = (botColorNum + 1) % 18;
                    potential_bot_id++;
                }
            }
            zmqThread.setBots(bots);
        }
	}
    switch (vidID) {
        case 0:
            displayedImage = colorImg;
            label = "Color Image";
            break;
        case 1:
            displayedImage = grayBg;
            label = "Gray Bg Image";
            break;
        case 2:
            displayedImage = grayImage;
            label = "Gray Image";
            break;
        case 3:
            displayedImage = grayDiff;
            label = "Subtracted Image";
            break;
        default:
            displayedImage = colorImg;
            label = "Color Image";
            break;
    }
}

//--------------------------------------------------------------
void testApp::draw()
{
	ofBackground(100,100,100);
    displayedImage.draw(0,0);
    ofBeginShape();
    ofSetColor(ofColor::seaGreen);
        for (int i = 0; i < line.getVertices().size(); i++)
        {
            ofVertex(line.getVertices().at(i));
        }
    ofEndShape();
    ofSetColor(ofColor::white);
    line.close();
    line.draw();
    // Create a String stream that will be used to display various information.
    stringstream reportStr;
    if (drawReportStringToggle){
        reportStr   << "3Pi Tracking" << endl
                    << "press ' ' to capture bg" << endl
                    << "number of bots found " << bots.size()
                    << ", fps: " << ofGetFrameRate() << endl << endl
                    << "Bot:\t(x,y)\tvel" << endl;
        ofDrawBitmapString(reportStr.str(), displayedImage.width, 20);
        reportStr.str("");
    }


    ofPoint bot_center_pt,
            info(displayedImage.width, 100);

    //  iterate through all the bots
    for (map<int, Bot>::iterator it = bots.begin(); it != bots.end(); it++) {
        ofSetColor(it->second.color);

        //  draw path of bot
        if (drawBotPathToggle)
            it->second.path.draw();


        //  draw blob and bounding rectangle
        if (drawBotRectToggle)
        {
            ofNoFill();
            ofSetLineWidth(4);
            ofRect(it->second.rect);
            it->second.getBlob().draw();
        }

        ofFill();

        //  create point at location of corresponding information tuple
        bot_center_pt = it->second.getCenter();
        //  add bot information to string stream
        ofSetColor(it->second.color);
        
        if (drawReportStringToggle)
        {
            drawInfoStrings(it->second.infoString(), info);
            
            //  draw associating lines from bot to information tuple
            if (drawAssociationToggle)
            {
                ofSetColor(it->second.color);
                drawAssociation(bot_center_pt, info);
            }
        }
        else
        {
            ofSetWindowShape(displayedImage.width, displayedImage.height);
        }

        info.y+=20;
        //  Draw ID on top of each bot
        if (drawBotIDToggle) {
            ofSetColor(ofColor::fuchsia);
            ofDrawBitmapString(ofToString(it->first) , bot_center_pt);
        }
    }
    if (showGUI) {
        gui.setPosition(0, 0);
        gui.draw();
    }

    //  Draw the label
    ofSetColor(ofColor::silver, 100);
    ofDrawBitmapString(label, 0, 10);

	ofSetHexColor(0xffffff);
}

//--------------------------------------------------------------
void testApp::exit()
{
    showColorImage.removeListener(this, &testApp::showColorPressed);
    showGrayImage.removeListener(this, &testApp::showGrayPressed);
    showBackgroundImage.removeListener(this, &testApp::showBGPressed);
    showSubtractedImage.removeListener(this, &testApp::showSubtractedPressed);
    cout << "listeners removed" << endl;
    
    zmqThread.stop();
    cout << "ZMG thread stopped" << endl;
}


//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch (key){
		case ' ':
			bLearnBakground = true;
            bots.clear();
			break;
        case 'g':
            showGUI = !showGUI;
			break;
	}
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    mouse_point.x = x;
    mouse_point.y = y;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){
    if (button == OF_MOUSE_BUTTON_1) {
//        ofSendMessage("Hello");
        //  Catch the Bot!
        for (vector<ofxCvBlob>::iterator blob_it = blobs.begin(); blob_it != blobs.end(); blob_it++) {
            if (blob_it->boundingRect.inside(x, y)) {
                bots.insert(pair<int, Bot>(potential_bot_id, Bot(*blob_it, potential_bot_id, colors[botColorNum])));
                botColorNum = (botColorNum + 1) % 18;
                potential_bot_id++;
                blobs.erase(blob_it);
                break;
            }
        }
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){
}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){
//    cout << "got the message: \"" << msg.message << "\"" << endl;
}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
void testApp::showColorPressed(){
    vidID = 0;
}

//--------------------------------------------------------------
void testApp::showBGPressed(){
    vidID = 1;
}

//--------------------------------------------------------------
void testApp::showGrayPressed(){
    vidID = 2;
}

//--------------------------------------------------------------
void testApp::showSubtractedPressed(){
    vidID = 3;
}

//--------------------------------------------------------------
void testApp::drawInfoStrings(string s, ofPoint& info ){
    ofVec2f p1 = ofVec2f(info.x, info.y-6);
    ofCircle(p1, 5);
    ofSetWindowShape(displayedImage.width + 300, displayedImage.height);
    ofRect(info.x, info.y-15, 300, 20);
    ofSetColor(ofColor::black);
    ofDrawBitmapString(s, info);
}

//--------------------------------------------------------------
void testApp::drawAssociation(ofPoint& botCenter, ofPoint& pt){
    ofVec2f p1(pt.x, pt.y-5);
    ofSetLineWidth(2);
    ofLine(p1, botCenter);
    ofCircle(botCenter, 10);
}

void testApp::updateBotsOnZMQThread(int &isAbleToUpdate)
{
    zmqThread.setBots(bots);
}
