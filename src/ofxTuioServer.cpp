/*
 TUIO Server Wrapper for OpenFrameworks
 Copyright (c) 2023 Katsuya Endoh <studio@enkatsu.org>
 Based on the TUIO Server Wrapper for OpenFrameworks by Matthias Dörfelt:
 
 TUIO Server Wrapper for OpenFrameworks
 Copyright (c) 2009 Matthias Dörfelt <info@mokafolio.de>
 Based on the TUIO SimpleSimulator by Martin Kaltenbrunner:
 
 Copyright (c) 2005-2009 Martin Kaltenbrunner <mkalten@iua.upf.edu>
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ofxTuioServer.h"

ofxTuioServer::ofxTuioServer()
{
}

//start Server (host, port);
void  ofxTuioServer::start(char * host, int port)
{
    verbose = false;
    tuioServer = new TuioServer(host, port);
    currentTime = TuioTime::getSessionTime();
}

//add cursor, object, blob
TuioCursor * ofxTuioServer::addCursor(float _x, float _y)
{
    if (verbose) cout << "ofxTuioServer: TUIO Cursor added at x: " + ofToString(_x) + " y: " + ofToString(_y) << endl;
    return tuioServer->addTuioCursor(_x, _y);
}

TuioObject * ofxTuioServer::addObject(int _sid, float _x, float _y, float _a)
{
    if (verbose) cout << "ofxTuioServer: TUIO Object added at x: " + ofToString(_x) + " y: " + ofToString(_y) << endl;
    return tuioServer->addTuioObject(_sid, _x, _y, _a);
}

TuioBlob * ofxTuioServer::addBlob(float _x, float _y, float _a, float _w, float _h, float _f)
{
    if (verbose) cout << "ofxTuioServer: TUIO Blob added at x: " + ofToString(_x) + " y: " + ofToString(_y) << endl;
    return tuioServer->addTuioBlob(_x, _y, _a, _w, _h, _f);
}

//update them
void ofxTuioServer::updateCursor(TuioCursor * _tcur, float _nx, float _ny)
{
    if (verbose) cout << "ofxTuioServer: update TUIO Cursor " << _tcur->getCursorID() << " to x: " + ofToString(_nx) + " y: " + ofToString(_ny) << endl;
    tuioServer->updateTuioCursor(_tcur, _nx, _ny);
}

void ofxTuioServer::updateObject(TuioObject * _tobj, float _nx, float _ny, float _ang)
{
    if (verbose) cout << "ofxTuioServer: update TUIO Object " << _tobj->getSymbolID() << " to x: " + ofToString(_nx) + " y: " + ofToString(_ny) << endl;
    tuioServer->updateTuioObject(_tobj, _nx, _ny, _ang);
}

void ofxTuioServer::updateBlob(TuioBlob * _tblb, float _x, float _y, float _a, float _w, float _h, float _f)
{
    if (verbose) cout << "ofxTuioServer: update TUIO Blob " << _tblb->getBlobID() << " to x: " + ofToString(_x) + " y: " + ofToString(_y) << endl;
    tuioServer->updateTuioBlob(_tblb, _x, _y, _a, _w, _h, _f);
}

//remove them
void ofxTuioServer::removeCursor(TuioCursor * _tcur)
{
    if(verbose) cout << "ofxTuioServer: remove TUIO Cursor " << _tcur->getCursorID() << endl;
    tuioServer->removeTuioCursor(_tcur);
}

void ofxTuioServer::removeObject(TuioObject * _tobj)
{
    if(verbose) cout << "ofxTuioServer: remove TUIO Cursor " << _tobj->getSymbolID() << endl;
    tuioServer->removeTuioObject(_tobj);
}

void ofxTuioServer::removeBlob(TuioBlob *_tblb)
{
    if(verbose) cout << "ofxTuioServer: remove TUIO Cursor " << _tblb->getBlobID() << endl;
    tuioServer->removeTuioBlob(_tblb);
}

//Getters
TuioTime ofxTuioServer::getCurrentTime()
{
    return tuioServer->getFrameTime();
}

void ofxTuioServer::setVerbose(bool _b)
{
    verbose = _b;
}

//send the OSC messages
void ofxTuioServer::run()
{
    //this is weird, it should actually be something like....
    tuioServer->stopUntouchedMovingCursors();
    tuioServer->commitFrame();
    currentTime = TuioTime::getSessionTime();
    tuioServer->initFrame(currentTime);
    //...this:
    /*
     currentTime = TuioTime::getSessionTime();
     tuioServer->initFrame(currentTime);
     tuioServer->stopUntouchedMovingCursors();
     tuioServer->commitFrame();
     */
    
    //propably due to the order of which OF handles mouse and keyboard events
}

//draw them for debug purposes
void ofxTuioServer::drawCursors()
{
    char id[3];
    // draw the cursors
    std::list<TuioCursor*> cursorList = tuioServer->getTuioCursors();
    for (std::list<TuioCursor*>::iterator tuioCursor = cursorList.begin(); tuioCursor!=cursorList.end(); tuioCursor++) {
        TuioCursor * tcur = (*tuioCursor);
        std::list<TuioPoint> path = tcur->getPath();
        if (path.size() > 0) {
            
            TuioPoint last_point = path.front();
            glBegin(GL_LINES);
            glColor3f(0.0, 0.0, 1.0);
            
            for (std::list<TuioPoint>::iterator point = path.begin(); point!=path.end(); point++) {
                glVertex3f(last_point.getX()*ofGetWidth(), last_point.getY()*ofGetHeight(), 0.0f);
                glVertex3f(point->getX()*ofGetWidth(), point->getY()*ofGetHeight(), 0.0f);
                last_point.update(point->getX(),point->getY());
            }
            glEnd();
            
            // draw the finger tip
            glColor3f(0.0, 0.75, 0.75);
            ofDrawCircle(tcur->getX() * ofGetWidth(), tcur->getY() * ofGetHeight(), 10);
        }
    }
}

void ofxTuioServer::drawObjects()
{
    std::list<TuioObject*> objectList = tuioServer->getTuioObjects();
    list<TuioObject*>::iterator tobj;
    
    for (tobj=objectList.begin(); tobj != objectList.end(); tobj++) {
        TuioObject *obj = (*tobj);
        glColor3f(1.0, 0.0, 0.0);
        glPushMatrix();
        glTranslatef(obj->getX() * ofGetWidth(), obj->getY() * ofGetHeight(), 0.0);
        glRotatef(obj->getAngleDegrees(), 0.0, 0.0, 1.0);
        ofDrawRectangle(-10.0, -10.0, 20.0, 20.0);
        glColor3f(1.0,1.0,1.0);
        ofDrawLine(0, 0, 0, -10);
        glPopMatrix();
        string str = "SymbolId: " + ofToString((int)(obj->getSymbolID()));
        ofDrawBitmapString(str, obj->getX() * ofGetWidth() - 10.0, obj->getY()*ofGetHeight() + 25.0);
        str = "SessionId: " + ofToString((int)(obj->getSessionID()));
        ofDrawBitmapString(str, obj->getX() * ofGetWidth() - 10.0, obj->getY()*ofGetHeight() + 40.0);
    }
    
}

TuioObject * ofxTuioServer::getTuioObject(int _sid)
{
    return tuioServer->getTuioObject(_sid);
}

std::list<TuioObject*> ofxTuioServer::getTuioObjects()
{
    return tuioServer->getTuioObjects();
}

TuioCursor * ofxTuioServer::getTuioCursor(int _sid)
{
    return tuioServer->getTuioCursor(_sid);
}
