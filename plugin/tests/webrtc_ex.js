var gWebrtc = null;
var gPeerConnection = null;
var gAcceptsIncomingCalls = true;
var gOurPeerId = null;
var gOurClientName = null;
var gServerUrl = null;
var gRemotePeerId = null;
var gAutoAddLocalToPeerConnectionStreamWhenCalled = true;

/* for getUserMedia */
var gLocalStream = null;
var gRequestWebcamAndMicrophoneResult = 'not-called-yet';
var gAddStreamConstraints = {};

/* for offer/answer */
var gCreateOfferConstraints = {};
var gCreateAnswerConstraints = {};

/* for render */
var gLocalView = null;
var gRemoteView = null;


var STUN_SERVER = 'stun.l.google.com:19302';

var gDebugCallback = consoleLog_;

function GetXmlHttpObject() {
    var xmlHttp = null;
    try { /* Firefox, Opera 8.0+, Safari */
        xmlHttp = new XMLHttpRequest();
    }catch (e) { /* Internet Explorer */
        try {
            xmlHttp = new ActiveXObject("Msxml2.XMLHTTP");
        }catch (e) {
            xmlHttp = new ActiveXObject("Microsoft.XMLHTTP");
        }
    }

    if (!xmlHttp) {
        debug("Donot support XMLHttpRequest");
    }

    return xmlHttp;
}

function AddEvent(obj, name, func) {
    /* debug("window.addEventListener = " + window.addEventListener); */
    if (window.addEventListener) {
        obj.addEventListener(name, func, false);
    } else {
        obj.attachEvent("on"+name, func);
    }
}



/**
 * example: 
 *  (a) get local stream and send offer to remote peer
 *  (b) request remote stream 
 */
function test_ex1()
{
    /**
     * step1:   getUserMedia to get local stream
     *
     * step2:   connect to peerconnectionserver
     *
     * step3:   add/remove local stream
     *
     * step4:   negotiate
     *          (a) setCreateOfferConstraints()
     *          (b) setCreateAnswerConstraints()
     *          (c) negotiateCall()
     *
     */
}


/**
 * =========================================================
 * for message handling
 */

function sendToPeer(peer, message) {
    var messageToLog = message.sdp ? message.sdp : message;
    debug('Sending message ' + messageToLog + ' to peer ' + peer + '.');

    var request = GetXmlHttpObject();
    var url = gServerUrl + '/message?peer_id=' + gOurPeerId + '&to=' + peer;
    request.open('POST', url, false);
    request.setRequestHeader('Content-Type', 'text/plain');
    request.send(message);
}

function isDisconnected() {
    return gOurPeerId == null;
}

function acceptIncomingCallsAgain() {
    gAcceptsIncomingCalls = true;
}

function doNotAutoAddLocalStreamWhenCalled() {
    gAutoAddLocalToPeerConnectionStreamWhenCalled = false;
}

function getReadyState() {
    if (gPeerConnection == null)
        return 'no-peer-connection';

    return 'active';
}

function preparePeerConnection() {
    if (gPeerConnection != null) {
        debug('creating peer connection, but we already have one.');
        return;
    }

    gPeerConnection = createPeerConnection(STUN_SERVER);
    return;
}

function negotiateCall() {
    if (gPeerConnection == null) {
        debug('negotiating call, but we have no peer connection.');
        return;
    }
        
    if (gOurPeerId == null) {
        debug('negotiating call, but not connected.');
        return;
    }
          
    if (gRemotePeerId == null) {
        debug('negotiating call, but missing remote peer.');
        return;
    }

    setupCall(gPeerConnection);
    debug('ok-negotiating');
}

function hangUp() {
    if (gPeerConnection == null) {
        debug('hanging up, but has no peer connection');
        return;
    }
        
    if (getReadyState() != 'active') {
        debug('hanging up, but ready state is not active (no call up).');
        return;
    }
        
    sendToPeer(gRemotePeerId, 'BYE');
    closeCall_();
    gAcceptsIncomingCalls = false;
    debug('ok-call-hung-up');
}



/**
 * for local stream and remote stream
 */

function addLocalStream() {
    if (gPeerConnection == null) {
        debug('adding local stream, but we have no peer connection.');
        return;
    }

    addLocalStreamToPeerConnection(gPeerConnection);
    debug('ok-added');
}

function removeLocalStream() {
    if (gPeerConnection == null) {
        debug('attempting to remove local stream, but no call is up');
        return;
    }

    removeLocalStreamFromPeerConnection(gPeerConnection);
    debug('ok-local-stream-removed');
}

function toggleRemoteStream(selectAudioOrVideoTrack, typeToToggle) {
    if (gPeerConnection == null) {
        debug('Tried to toggle remote stream, ' + 'but have no peer connection.');
        return;
    }
    if (gPeerConnection.getRemoteStreams().length == 0) {
        debug('Tried to toggle remote stream, ' + 'but not receiving any stream.');
        return;
    }

    var track = selectAudioOrVideoTrack(gPeerConnection.getRemoteStreams()[0]);
    toggle_(track, 'remote', typeToToggle);
}

function toggleLocalStream(selectAudioOrVideoTrack, typeToToggle) {
    if (gPeerConnection == null) {
        debug('Tried to toggle local stream, ' + 'but have no peer connection.');
        return;
    }
    if (gPeerConnection.getLocalStreams().length == 0) {
        debug('Tried to toggle local stream, but there is no local ' +  'stream in the call.');
        return;
    }

    var track = selectAudioOrVideoTrack(gPeerConnection.getLocalStreams()[0]);
    toggle_(track, 'local', typeToToggle);
}

function toggleLocalStream(selectAudioOrVideoTrack, typeToToggle) {
    if (gPeerConnection == null) {
        debug('Tried to toggle local stream, ' + 'but have no peer connection.');
        return;
    }

    if (gPeerConnection.getLocalStreams().length == 0) {
        debug('Tried to toggle local stream, but there is no local ' + 'stream in the call.');
        return;
    }

    var track = selectAudioOrVideoTrack(gPeerConnection.getLocalStreams()[0]);
    toggle_(track, 'local', typeToToggle);
}

function toggle_(track, localOrRemote, audioOrVideo) {
    if (!track) {
        debug('Tried to toggle ' + localOrRemote + ' ' + audioOrVideo + ' stream, but has no such stream.');
        return;
    }

    track.enabled = !track.enabled;
    debug('ok-' + audioOrVideo + '-toggled-to-' + track.enabled);
}



/** 
 * @public: for getUserMedia
 */
function getUserMedia(constraints)
{
    debug('Requesting getUserMedia, constraints: ' + constraints);
    try {
        gWebrtc.getUserMedia(constraints, 
                getUserMediaOkCallback_,
                getUserMediaFailedCallback_);
    } catch (exception) {
        debug('getUserMedia says: ' + exception);
    }
}

function getUserMediaOkCallback_(stream) {
    debug("getUserMediaOkCallback_ for gLocalStream");
    gLocalStream = stream;
    gRequestWebcamAndMicrophoneResult = 'ok-got-stream';
}

function getUserMediaFailedCallback_(error) {
    debug('GetUserMedia FAILED: Maybe the camera is in use by another process?');
    gRequestWebcamAndMicrophoneResult = 'failed-with-error-' + error.code;
    debug(gRequestWebcamAndMicrophoneResult);
}

function obtainGetUserMediaResult() {
    debug(gRequestWebcamAndMicrophoneResult);
    return gRequestWebcamAndMicrophoneResult;
}

function addLocalStreamToPeerConnection(peerConnection) {
    if (gLocalStream == null) {
        debug('Tried to add local stream to peer connection, ' + 'but there is no stream yet.');
        return;
    }

    try {
        debug('Added local stream.');
        peerConnection.addStream(gLocalStream, gAddStreamConstraints);
    } catch (exception) {
        debug('Failed to add stream with constraints ' + gAddStreamConstraints + ': ' + exception);
        return;
    }

    try {
        debug("setLocalRenderer for add");
        // setRenderer(gLocalView, peerConnection.setLocalRenderer, 0);
        istream = gLocalStream.getStreamX();
        irender = gLocalView.getRendererX();
        addRenderer(peerConnection, istream, irender);
    } catch (exception) {
        debug("failed to setLocalRenderer(0): " + exception);
    }
}

function removeLocalStreamFromPeerConnection(peerConnection) {
    if (gLocalStream == null) {
        debug('Tried to remove local stream from peer connection, ' + 'but there is no stream yet.');
        return;
    }

    try {
        debug("setLocalRenderer for remove");
        //setRenderer(gLocalView, peerConnection.setLocalRenderer, 1);
        istream = gLocalStream.getStreamX();
        irender = gLocalView.getRendererX();
        removeRenderer(peerConnection, istream, irender);
    } catch (exception) {
        debug("failed to setLocalRenderer(1): " + exception);
    }

    try {
        peerConnection.removeStream(gLocalStream);
    } catch (exception) {
        debug('Could not remove stream: ' + exception);
        return;
    }
    debug('Removed local stream.');
}



/**
 * @public
 * Connects to the provided peerconnection_server.
 * serverUrl: http://localhost:8888
 */
function connect(serverUrl, clientName) {
    if (gOurPeerId != null) {
        debug('connecting, but is already connected.');
        return;
    }

    gServerUrl = serverUrl;
    gOurClientName = clientName;

    var callUrl = serverUrl + '/sign_in?' + clientName;
    debug('Connecting to ' + callUrl);

    var request = GetXmlHttpObject();
    request.onreadystatechange = function() {
        connectCallback_(request);
    }
    request.open('GET', callUrl, true);
    request.send();
}

function connectCallback_(request) {
    debug('Connect callback: ' + request.status + ', ' + request.readyState);
    if (request.status == 0) {
        debug('peerconnection_server doesn\'t seem to be up.');
        return;
    }

    if (request.readyState == 4 && request.status == 200) {
        gOurPeerId = parseOurPeerId_(request.responseText);
        gRemotePeerId = parseRemotePeerIdIfConnected_(request.responseText);
        debug("gOurPeerId = " + gOurPeerId + ", gRemotePeerId = " + gRemotePeerId);
        startHangingGet_(gServerUrl, gOurPeerId);
        return;
    }
}

function startHangingGet_(server, ourId) {
    if (isDisconnected()) {
        debug("gOurPeerId is NULL");
        return;
    }

    var hangingGetRequest = GetXmlHttpObject();
    hangingGetRequest.onreadystatechange = function() {
        hangingGetCallback_(hangingGetRequest, server, ourId);
    }
    hangingGetRequest.ontimeout = function() {
        hangingGetTimeoutCallback_(hangingGetRequest, server, ourId);
    }

    callUrl = server + '/wait?peer_id=' + ourId;
    debug('Sending ' + callUrl);
    hangingGetRequest.open('GET', callUrl, true);
    hangingGetRequest.send();
}

function hangingGetCallback_(hangingGetRequest, server, ourId) {
    if (hangingGetRequest.readyState != 4) {
        debug("hangingGetRequest.readyState is not 4");
        return;
    }

    if (hangingGetRequest.status == 0) {
        /* Code 0 is not possible if the server actually responded. */
        debug('Previous request was malformed, or server is unavailable.');
        return;
    }

    if (hangingGetRequest.status != 200) {
        debug('Error ' + hangingGetRequest.status + ' from server: ' + hangingGetRequest.statusText);
        return;
    }

    var targetId = readResponseHeader_(hangingGetRequest, 'Pragma');
    debug("targetId = " + targetId + ", ourId = " + ourId);
    if (targetId == ourId)
        handleServerNotification_(hangingGetRequest.responseText);
    else
        handlePeerMessage_(targetId, hangingGetRequest.responseText);

    hangingGetRequest.abort();
    restartHangingGet_(server, ourId);
}

function hangingGetTimeoutCallback_(hangingGetRequest, server, ourId) {
    debug('Hanging GET times out, re-issuing...');
    hangingGetRequest.abort();
    restartHangingGet_(server, ourId);
}

function restartHangingGet_(server, ourId) {
    window.setTimeout(function() {
        startHangingGet_(server, ourId);
    }, 0);
}

function handleServerNotification_(message) {
    var parsed = message.split(',');
    if (parseInt(parsed[2]) == 1) {
        /* Peer connected - this must be our remote peer, and it must mean we
           connected before them (except if we happened to connect to the server
           at precisely the same moment).
        */
        debug('Found remote peer with name ' + parsed[0] + ', id ' +
                parsed[1] + ' when connecting.');
        gRemotePeerId = parseInt(parsed[1]);
    }
}

function closeCall_() {
    if (gPeerConnection == null) {
        debug('Closing call, but no call active.');
        return;
    }
    gPeerConnection.close();
    gPeerConnection = null;
}

function handlePeerMessage_(peerId, message) {
    debug('Received message from peer ' + peerId + ': ' + message);
    if (peerId != gRemotePeerId) {
        debug('Received notification from unknown peer ' + peerId + ' (only know about ' + gRemotePeerId + '.');
        return;
    }

    if (message.search('BYE') == 0) {
        debug('Received BYE from peer: closing call');
        closeCall_();
        return;
    }

    if (gPeerConnection == null && gAcceptsIncomingCalls) {
        /* The other side is calling us. */
        debug('We are being called: answer...');
        gPeerConnection = createPeerConnection(STUN_SERVER);
        if (gAutoAddLocalToPeerConnectionStreamWhenCalled && obtainGetUserMediaResult() == 'ok-got-stream') {
            debug('We have a local stream, so hook it up automatically.');
            addLocalStreamToPeerConnection(gPeerConnection);
        }
        answerCall(gPeerConnection, message);
        return;
    }
    handleMessage(gPeerConnection, message);
}



/**
 * ======================================================
 * for utils
 */

function readResponseHeader_(request, key) {
    var value = request.getResponseHeader(key);
    if (value == null || value.length == 0) {
        debug('Received empty value ' + value + ' for response header key ' + key + '.');
        return -1;
    }
    return parseInt(value);
}

function parseOurPeerId_(responseText) {
    /* According to peerconnection_server's protocol. */
    var peerList = responseText.split('\n');
    return parseInt(peerList[0].split(',')[1]);
}

function parseRemotePeerIdIfConnected_(responseText) {
    var peerList = responseText.split('\n');
    if (peerList.length == 1) {
        /* No peers have connected yet - we'll get their id later in a notification. */
        return null;
    }

    var remotePeerId = null;
    for (var i = 0; i < peerList.length; i++) {
        if (peerList[i].length == 0)
            continue;

        var parsed = peerList[i].split(',');
        var name = parsed[0];
        var id = parsed[1];

        if (id != gOurPeerId) {
            debug('Found remote peer with name ' + name + ', id ' + id + ' when connecting.');
            /* There should be at most one remote peer in this test.*/
            if (remotePeerId != null) {
                debug('Expected just one remote peer in this test: ' +  'found several.');
                return null;
            }

            /* Found a remote peer. */
            remotePeerId = id;
        }
    }

    return remotePeerId;
}

function consoleLog_(message) {
    /* It is not legal to treat console.log as a first-class object, so wrap it. */
    console.log(message);
}

function replaceDebugCallback(callback) {
    gDebugCallback = callback;
}

function debug(txt) {
    if (gOurClientName == null)
        prefix = '';
    else
        prefix = gOurClientName + ' says: ';

    gDebugCallback(prefix + txt);
}

function success_(method) {
    debug(method + '(): success.');
}

function failure_(method, error) {
    debug(method + '() failed: ' + error);
}


/**
 * ======================================================
 *  for jsep peerconnection
 */

var gTransformOutgoingSdp = function(sdp) { return sdp; };

function setOutgoingSdpTransform(transformFunction) {
    gTransformOutgoingSdp = transformFunction;
}

function setCreateAnswerConstraints(mediaConstraints) {
    gCreateAnswerConstraints = mediaConstraints;
}

function setCreateOfferConstraints(mediaConstraints) {
    gCreateOfferConstraints = mediaConstraints;
}

function handleMessage(peerConnection, message) {
    var parsed_msg = JSON.parse(message);
    if (parsed_msg.type) {
        debug("handleMessage for RTCSessionDescription");
        var session_description = gWebrtc.createRTCSessionDescription();
        debug("set_type");
        session_description.type = parsed_msg.type;
        if (parsed_msg.sdp) {
            debug("set_sdp");
            session_description.sdp = parsed_msg.sdp;
        }
        debug("setRemoteDescription")
        peerConnection.setRemoteDescription(
                session_description,
                function() { success_('setRemoteDescription'); },
                function() { failure_('setRemoteDescription'); });
        if (session_description.type == 'offer') {
            debug('createAnswer with constraints: ' + JSON.stringify(gCreateAnswerConstraints, null, ' '));
            peerConnection.createAnswer(
                    session_description,
                    setLocalAndSendMessage_,
                    function() { failure_('createAnswer'); },
                    gCreateAnswerConstraints);
        }
        debug("end for rtcSdp");
        return;
    } else if (parsed_msg.candidate) {
        debug("handleMessage for RTCIceCandidate");
        var candidate = gWebrtc.createRTCIceCandidate();
        candidate.candidate = parsed_msg.candidate;
        if (parsed_msg.sdpMid) {
            debug("set_sdpMid");
            candidate.sdpMid = parsed_msg.sdpMid;
        }
        if (parsed_msg.sdpMLineIndex) {
            debug("set_sdpMLineIndex");
            candidate.sdpMLineIndex = parsed_msg.sdpMLineIndex;
        }
        peerConnection.addIceCandidate(candidate);
        return;
    }
    debug('unknown message received');
    return;
}

function createPeerConnection(stun_server) {
    servers = {iceServers: [{url: 'stun:' + stun_server}]};
    try {
        peerConnection = gWebrtc.createRTCPeerConnection();
    } catch (exception) {
        debug('Failed to create peer connection: ' + exception);
        return null;
    }

    AddEvent(peerConnection, "onnegotationneeded", onnegotationneededCallback_);
    AddEvent(peerConnection, "onicecandidate", onicecandidateCallback_);
    AddEvent(peerConnection, "onopen", onopenCallback_);
    AddEvent(peerConnection, "onstatechange", onstatechangeCallback_);
    AddEvent(peerConnection, "onicechange", onicechangeCallback_);
    AddEvent(peerConnection, "onaddstream", onaddstreamCallback_);
    AddEvent(peerConnection, "onremovestream", onremovestreamCallback_);
    return peerConnection;
}

function setupCall(peerConnection) {
    debug('createOffer with constraints: ' + JSON.stringify(gCreateOfferConstraints, null, ' '));
    peerConnection.createOffer(
            setLocalAndSendMessage_,
            function() { failure_('createOffer'); },
            gCreateOfferConstraints);
    debug('setupCall end');
}

function answerCall(peerConnection, message) {
    handleMessage(peerConnection, message);
}

function setLocalAndSendMessage_(session_description) {
    debug("setLocalAndSendMessage_");
    session_description.sdp = gTransformOutgoingSdp(session_description.sdp);
    peerConnection.setLocalDescription(
            session_description,
            function() { success_('setLocalDescription'); },
            function() { failure_('setLocalDescription'); });
    var json_offer = {
        type: session_description.type,
        sdp: session_description.sdp
    };
    /* debug('sending SDP message:\n' + JSON.stringify(json_offer)); */
    /* var parsed_msg = JSON.parse(JSON.stringify(json_offer)); */
    sendToPeer(gRemotePeerId, JSON.stringify(json_offer));
}



/**
 * callback 
 */
function onnegotationneededCallback_() {
    debug("onnegotationneeded");
}

function onicecandidateCallback_(event) {
    debug("onicecandidate");
    if (event.candidate) {
        var json_candidate = {
            candidate: event.candidate,
            sdpMid: event.sdpMid,
            sdpMLineIndex: event.sdpMLineIndex
        };
        sendToPeer(gRemotePeerId, JSON.stringify(json_candidate));
    }
}

function onopenCallback_() {
    debug("onopen");
}

function onstatechangeCallback_(istate) {
    debug("onstatechange");
}

function onicechangeCallback_(istate) {
    debug("onicechange");
}

function onaddstreamCallback_(istream) {
    debug('onaddstream (remote stream)');
    try {
        debug("setRemoteRenderer for add");
        //setRenderer(gRemoteView, gPeerConnection.setRemoteRenderer, 0);
        irender = gRemoteView.getRendererX();
        addRenderer(gPeerConnection, istream, irender);
    } catch (exception) {
        debug("failed to addRenderer: " + exception);
    }

    /*var videoTag = document.getElementById('remote-view');
      videoTag.src = webkitURL.createObjectURL(event.stream);
    */

    /* Due to crbug.com/110938 the size is 0 when onloadedmetadata fires.
       videoTag.onloadedmetadata = displayVideoSize_(videoTag);
       Use setTimeout as a workaround for now.
       Displays the remote video size for both the video element and the stream.
    */
    /* setTimeout(function() {displayVideoSize_(videoTag);}, 500); */
}

function onremovestreamCallback_(istream) {
    debug('onremovestream (remote stream)');
    /* document.getElementById('remote-view').src = ''; */
    try {
        debug("setRemoteRenderer for remove");
        //setRenderer(gRemoteView, gPeerConnection.setRemoteRenderer, 1);
        irender = gRemoteView.getRendererX();
        removeRenderer(gPeerConnection, istream, irender);
    } catch (exception) {
        debug("failed to removeRenderer: " + exception);
    }
}


function setRenderer(view, pcSetRenderer, action) {
    var render;
    try {
        render = view.getRendererX();
    } catch (exception) {
        debug("failed to getRenderer: " + exception);
        return;
    }
    debug("================ renderX=" + render);

    try {
        pcSetRenderer(render, action);
    } catch (exception) {
        debug("failed to setRenderer: " + exception);
        return;
    }
}

function addRenderer(pc, istream, irender) {
    debug("[=====]addRenderer, istream: " + istream + ", irender: " + irender);
    try {
        pc.addRenderer(istream, irender);
    } catch (exception) {
        debug("failed to addRenderer: " + exception);
        return;
    }
}

function removeRenderer(pc, istream, irender) {
    try {
        pc.removeRenderer(istream, irender);
    } catch (exception) {
        debug("failed to removeRenderer: " + exception);
        return;
    }
}

