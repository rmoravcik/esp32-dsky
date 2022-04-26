#ifdef ESP32
#include <Update.h>

#include "OTA.h"

const char* handleOta =
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<title>OTA</title>"
"<style>"
   "body {"
   "  background-color: #d2f3eb;"
   "  font-family: Arial, Helvetica, Sans-Serif;"
   "  Color: #000000;"
   "  font-size:12pt;"
   "  width:320px;"
   "}"
   ".titel {"
   "font-weight:bold;"
   "text-align:center;"
   "width:100%;"
   "padding:5px;"
   "}"
   ".zeile {"
   "  width:100%;"
   "  padding:5px;"
   "  text-align: center;"
   "}"
   "input {"
   "font-size:14pt;"
   "width:150px;"
   "border-radius:10px;"
   "margin:5px;"
   "}"
"</style>"
"<div id='main_div' style='margin-left:15px;margin-right:15px;'>"
"<div class='titel'>OTA ESP32-DSKY</div>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<div class='zeile'><input type='file' name='update'>"
      "<input type='submit' value='Update'>"
   "</div>"
"</form>"
"<div class='zeile' id='prg'>Progress: 0%</div>"
"<script>"
   "$('form').submit(function(e){"
   "e.preventDefault();"
   "var form = $('#upload_form')[0];"
   "var data = new FormData(form);"
   " $.ajax({"
   "url: '/update',"
   "type: 'POST',"
   "data: data,"
   "contentType: false,"
   "processData:false,"
   "xhr: function() {"
   "var xhr = new window.XMLHttpRequest();"
   "xhr.upload.addEventListener('progress', function(evt) {"
   "if (evt.lengthComputable) {"
   "var per = evt.loaded / evt.total;"
   "$('#prg').html('Progress: ' + Math.round(per*100) + '%');"
   "}"
   "}, false);"
   "return xhr;"
   "},"
   "success:function(d, s) {"
   "console.log('Success!')"
   "},"
   "error: function (a, b, c) {"
   "}"
   "});"
   "});"
"</script>";

OTA::OTA(AsyncWebServer *server)
{
  m_server = server;

  m_server->on("/ota", HTTP_GET, [&](AsyncWebServerRequest *request) {
    request->send(200, "text/html", handleOta);
  });

  m_server->on("/update", HTTP_POST, [&](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, [&](AsyncWebServerRequest *request, const String& filename, size_t index, uint8_t *data, size_t len, bool final) {
    if (!index) {
      Serial.printf("Update: %s\n", filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }      
    }

    if (len) {
      if (Update.write(data, len) != len) {
        Update.printError(Serial);
      }
    }

    if (final) {
      if (Update.end(true)) {
        Serial.printf("Update Success: %u\nRebooting...\n", len);
      } else {
        Update.printError(Serial);
      }
    }
  });
}

OTA::~OTA()
{
}
#endif
