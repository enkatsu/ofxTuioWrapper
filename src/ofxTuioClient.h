/*
 TUIO Client Wrapper for OpenFrameworks
 Copyright (c) 2023 Katsuya Endoh <studio@enkatsu.org>
 Based on the TUIO Client Wrapper for OpenFrameworks by Matthias Dörfelt:
 
 TUIO Client Wrapper for OpenFrameworks
 Copyright (c) 2009 Matthias Dörfelt <info@mokafolio.de>
 Based on the TUIO Demo by Martin Kaltenbrunner:
 
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

#ifndef _TUIO_CLIENT
#define _TUIO_CLIENT

#include "ofMain.h"
#include "TuioListener.h"
#include "TuioClient.h"
#include "TuioObject.h"
#include "TuioCursor.h"
#include "TuioPoint.h"

#include <list>
#include <math.h>

using namespace TUIO;

class ofxTuioClient : public TuioListener {
    
public:
    ofxTuioClient();
    ~ofxTuioClient() {
        disconnect();
    }
    void update();
    void connect(int _port);
    void disconnect();
    
    void flipInput180(bool flip) {
        bFlip = flip;
    }
    
    void addTuioObject(TuioObject * tobj);
    void updateTuioObject(TuioObject * tobj);
    void removeTuioObject(TuioObject * tobj);
    
    void addTuioCursor(TuioCursor * tcur);
    void updateTuioCursor(TuioCursor * tcur);
    void removeTuioCursor(TuioCursor * tcur);
    
    void addTuioBlob(TuioBlob * tblb);
    void removeTuioBlob(TuioBlob * tblb);
    void updateTuioBlob(TuioBlob * tblb);
    
    void refresh(TuioTime frameTime);
    void drawCursors();
    void drawObjects();
    
    void setVerbose(bool b);
    
    ofEvent<ofTouchEventArgs> touchDown;
    ofEvent<ofTouchEventArgs> touchMoved;
    ofEvent<ofTouchEventArgs> touchUp;
    
    //OF POCO EVENTS TO FORWARD TUIO EVENTS TO testApp or any other Class
    
    ofEvent<TuioObject> objectAdded;
    ofEvent<TuioObject> objectRemoved;
    ofEvent<TuioObject> objectUpdated;
    
    ofEvent<TuioCursor> cursorAdded;
    ofEvent<TuioCursor> cursorRemoved;
    ofEvent<TuioCursor> cursorUpdated;
    
    ofEvent<TuioBlob> blobAdded;
    ofEvent<TuioBlob> blobRemoved;
    ofEvent<TuioBlob> blobUpdated;
    
    TuioClient * client;
    
protected:
    bool bVerbose, bIsConnected, bFlip;
    ofThreadChannel<TuioObject> objectAddedQueue, objectRemovedQueue, objectUpdatedQueue;
    ofThreadChannel<ofTouchEventArgs> touchAddedQueue, touchRemovedQueue, touchUpdatedQueue;
    ofThreadChannel<TuioCursor> cursorAddedQueue, cursorRemovedQueue, cursorUpdatedQueue;
    ofThreadChannel<TuioBlob> blobAddedQueue, blobRemovedQueue, blobUpdatedQueue;
};
#endif
