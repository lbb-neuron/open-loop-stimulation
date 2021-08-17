# Code from: https://github.com/realpython/materials/blob/master/python-sockets-tutorial/libserver.py (191110)
# Some changes in:
# - _create_response
# - create_response
# - process_request
# - read
# - process_protoheader
# - __init__
# - _create_message
# - _write

import sys
import selectors
import json
import io
import struct

class Message:
    def __init__(self, selector, sock, addr):
        self.selector         = selector
        self.sock             = sock
        self.addr             = addr
        self._recv_buffer     = b""
        self._send_buffer     = b""
        self._jsonheader_len  = None
        self.jsonheader       = None
        self.request          = None
        self.response_created = False
        self.pattern          = [0]*258

    def _set_selector_events_mask(self, mode):
        """Set selector to listen for events: mode is 'r', 'w', or 'rw'."""
        if mode == "r":
            events = selectors.EVENT_READ
        elif mode == "w":
            events = selectors.EVENT_WRITE
        elif mode == "rw":
            events = selectors.EVENT_READ | selectors.EVENT_WRITE
        else:
            raise ValueError(f"Invalid events mask mode {repr(mode)}.")
        self.selector.modify(self.sock, events, data=self)

    def _read(self):
        try:
            # Should be ready to read
            data = self.sock.recv(4096)
        except BlockingIOError:
            # Resource temporarily unavailable (errno EWOULDBLOCK)
            pass
        else:
            if data:
                self._recv_buffer += data
            else:
                raise RuntimeError("Peer closed.")

    def _write(self):
        if self._send_buffer:
            try:
                sent = self.sock.send(self._send_buffer)
            except BlockingIOError:
                pass
            else:
                self._send_buffer = self._send_buffer[sent:]
                if sent and not self._send_buffer:
                    self._jsonheader_len = None
                    self.request = None
                    self._set_selector_events_mask("r")

    def _json_encode(self, obj, encoding):
        return json.dumps(obj, ensure_ascii=False).encode(encoding)

    def _json_decode(self, json_bytes, encoding):
        tiow = io.TextIOWrapper(io.BytesIO(json_bytes), encoding=encoding, newline="")
        obj = json.load(tiow)
        tiow.close()
        return obj

    
    def _create_message(self, *, content_bytes):
        message_hdr = struct.pack(">i", len(content_bytes))
        message = content_bytes
        return message

    def _create_response_json_content(self):
        action = self.request.get("action")
        if action == "search":
            query = self.request.get("value")
            answer = request_search.get(query) or f'No match for "{query}".'
            content = {"result": answer}
        else:
            content = {"result": f'Error: invalid action "{action}".'}
        content_encoding = "utf-8"
        response = {
            "content_bytes": self._json_encode(content, content_encoding),
            "content_type": "text/json",
            "content_encoding": content_encoding,
        }
        return response

    def _create_response_binary_content(self):
        response = {
            "content_bytes": b"First 10 bytes of request: "
            + self.request[:10],
            "content_type": "binary/custom-server-binary-type",
            "content_encoding": "binary",
        }
        return response

    def process_events(self, mask):
        if mask & selectors.EVENT_READ:
            self.read()
        if mask & selectors.EVENT_WRITE:
            self.write()

    def read(self):
        self._read()

        if self._jsonheader_len is None:
            self.process_protoheader()

        if self._jsonheader_len is not None:
            if self.jsonheader is None:
                self.process_request()

    def write(self):
        if self.request:
            if not self.response_created:
                self.create_response()

        self._write()

    def close(self):
        print("closing connection to", self.addr)
        try:
            self.selector.unregister(self.sock)
        except Exception as e:
            print(f"error: selector.unregister() exception for",f"{self.addr}: {repr(e)}",)

        try:
            self.sock.close()
        except OSError as e:
            print(f"error: socket.close() exception for",f"{self.addr}: {repr(e)}",)
        finally:
            # Delete reference to socket object for garbage collection
            self.sock = None

    def process_protoheader(self):
        hdrlen = 4
        if len(self._recv_buffer) >= hdrlen:
            self._jsonheader_len = struct.unpack(">i", self._recv_buffer[:hdrlen])[0]
            self._recv_buffer = self._recv_buffer[hdrlen:]

    def process_jsonheader(self):
        hdrlen = self._jsonheader_len
        if len(self._recv_buffer) >= hdrlen:
            self.jsonheader = self._json_decode(self._recv_buffer[:hdrlen],"utf-8")
            self._recv_buffer = self._recv_buffer[hdrlen:]
            for reqhdr in ("byteorder","content-length","content-type","content-encoding",):
                if reqhdr not in self.jsonheader:
                    raise ValueError(f'Missing required header "{reqhdr}".')

    def process_request(self):
        content_len = self._jsonheader_len
        if not len(self._recv_buffer) >= content_len:
            return
        data = self._recv_buffer[:content_len]
        self._recv_buffer = self._recv_buffer[content_len:]
        self.request = self._json_decode(data,"utf-8")
        self.response_created = False;
        
    def _create_response(self):
        content = {"pattern" : self.pattern}
        response = self._json_encode(content, "utf-8")
        return response
        
    def create_response(self):
        response = self._create_response()

        message = self._create_message(content_bytes=response)
        
        self.response_created = True
        self._send_buffer += message