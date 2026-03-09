#include "Context.h"


// -------------------------------------------------------
// Embedded HTML
// -------------------------------------------------------

static const char HTML_PAGE[] PROGMEM = R"rawhtml(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>SX1280 Control</title>
<style>
:root{--bg:#0d1117;--card:#161b22;--border:#30363d;--accent:#58a6ff;--green:#3fb950;--red:#f85149;--yellow:#e3b341;--text:#e6edf3;--sub:#8b949e}
*{box-sizing:border-box;margin:0;padding:0}
body{background:var(--bg);color:var(--text);font-family:'Segoe UI',system-ui,sans-serif;min-height:100vh;display:flex;flex-direction:column;align-items:center;padding:24px 16px}
h1{font-size:1.5rem;font-weight:700;margin-bottom:4px}
.sub{color:var(--sub);font-size:.85rem;margin-bottom:24px}
.card{background:var(--card);border:1px solid var(--border);border-radius:12px;padding:20px;width:100%;max-width:480px;margin-bottom:16px}
.card-title{font-size:.72rem;text-transform:uppercase;letter-spacing:.1em;color:var(--sub);margin-bottom:16px}
.row{display:flex;align-items:center;gap:12px;margin-bottom:14px}
.row:last-child{margin-bottom:0}
label{font-size:.88rem;min-width:72px;color:var(--sub)}
input[type=range]{flex:1;accent-color:var(--accent);cursor:pointer;height:6px}
.val{font-size:.95rem;font-weight:700;min-width:80px;text-align:right}
.btn{width:100%;padding:14px;border:none;border-radius:10px;font-size:1rem;font-weight:700;cursor:pointer;transition:filter .15s,transform .1s,opacity .2s;letter-spacing:.04em}
.btn:hover:not(:disabled){filter:brightness(1.15);transform:scale(1.02)}
.btn:active:not(:disabled){transform:scale(.98)}
.btn:disabled{opacity:.5;cursor:not-allowed}
.btn-signal-on{background:var(--green);color:#000}
.btn-signal-off{background:var(--red);color:#fff}
.sep{height:1px;background:var(--border);margin:14px 0}
.srow{display:flex;justify-content:space-between;align-items:center;font-size:.88rem;padding:8px 0;border-bottom:1px solid var(--border)}
.srow:last-child{border:none}
.sval{color:var(--text);font-weight:600}
.badge{padding:3px 11px;border-radius:20px;font-size:.78rem;font-weight:700}
.b-on{background:#1c4d27;color:var(--green)}
.b-off{background:#3d1a18;color:var(--red)}
.b-ap{background:#1a2a3d;color:var(--accent)}
#dot{display:inline-block;width:9px;height:9px;border-radius:50%;background:var(--sub);margin-right:7px;vertical-align:middle;transition:background .4s}
.dot-ok{background:var(--green) !important}
</style>
</head>
<body>
<h1>&#x1F4E1; SX1280 Control</h1>
<p class="sub">2.4 GHz LoRa Transceiver &mdash; CW Generator</p>

<div class="card">
  <div class="card-title">RF Settings</div>
  <div class="row">
    <label>Frequency</label>
    <input type="range" id="freqSlider" min="2400" max="2500" step="1" value="2450"
           oninput="onFreqInput(this.value)" onchange="applyRF()">
    <span class="val" id="freqVal">2450 MHz</span>
  </div>
  <div class="row">
    <label>Power</label>
    <input type="range" id="pwrSlider" min="-18" max="12" step="1" value="10"
           oninput="onPwrInput(this.value)" onchange="applyRF()">
    <span class="val" id="pwrVal">10 dBm</span>
  </div>
</div>

<div class="card">
  <div class="card-title">Signal Control</div>
  <button class="btn btn-signal-on" id="toggleBtn" onclick="toggleSignal()">
    &#x25B6; START SIGNAL
  </button>
</div>

<div class="card">
  <div class="card-title"><span id="dot"></span>Actual Device State</div>
  <div class="srow"><span>Signal</span>
    <span id="stSig" class="badge b-off">OFF</span></div>
  <div class="srow"><span>Frequency</span>
    <span class="sval" id="stFreq">&mdash;</span></div>
  <div class="srow"><span>Power</span>
    <span class="sval" id="stPwr">&mdash;</span></div>
  <div class="srow"><span>WiFi Clients</span>
    <span class="sval" id="stClients">&mdash;</span></div>
  <div class="srow"><span>AP Address</span>
    <span id="stIP" class="badge b-ap">192.168.4.1</span></div>
</div>

<script>
// ── Local state ──
var pendingFreq = 2450, pendingPwr = 10;
var actualSignal = false;

// Triggers instantly while dragging
function onFreqInput(v) {
  pendingFreq = +v;
  document.getElementById('freqVal').textContent = v + ' MHz';
}

function onPwrInput(v) {
  pendingPwr = +v;
  document.getElementById('pwrVal').textContent = v + ' dBm';
}

// Triggers when dragging is released (onchange)
function applyRF() {
  var btn = document.getElementById('toggleBtn');
  btn.disabled = true; // Block button until we get a response
  fetch('/set?freq='+pendingFreq+'&power='+pendingPwr+'&signal='+(actualSignal?1:0))
    .then(function(r){return r.json();}).then(updateStatus)
    .catch(function(){ btn.disabled = false; });
}

// Toggle signal
function toggleSignal() {
  var btn = document.getElementById('toggleBtn');
  btn.disabled = true; // Block button
  actualSignal = !actualSignal;
  fetch('/set?freq='+pendingFreq+'&power='+pendingPwr+'&signal='+(actualSignal?1:0))
    .then(function(r){return r.json();}).then(updateStatus)
    .catch(function(){ btn.disabled = false; });
}

// ── ONLY updates the status section ──
function updateStatus(d) {
  actualSignal = d.signal;

  var btn = document.getElementById('toggleBtn');
  btn.disabled = false; // Unblock the button since we got a fresh status update

  var sig = document.getElementById('stSig');
  if (actualSignal) {
    btn.textContent = '\u23F9 STOP SIGNAL';
    btn.className = 'btn btn-signal-off';
    sig.textContent = 'ON'; sig.className = 'badge b-on';
  } else {
    btn.textContent = '\u25B6 START SIGNAL';
    btn.className = 'btn btn-signal-on';
    sig.textContent = 'OFF'; sig.className = 'badge b-off';
  }

  document.getElementById('stFreq').textContent = d.freq + ' MHz';
  document.getElementById('stPwr').textContent  = d.power + ' dBm';
  document.getElementById('stClients').textContent = d.clients;
  document.getElementById('dot').className = d.clients > 0 ? 'dot-ok' : '';
}

function pollStatus() {
  fetch('/status')
    .then(function(r){return r.json();}).then(updateStatus).catch(function(){});
}

setInterval(pollStatus, 2000);
pollStatus();
</script>
</body>
</html>
)rawhtml";


std::unique_ptr<Context> gCtx;

void setup() 
{
  Serial.begin(115200);
  Serial.println();

  gCtx = std::make_unique<Context>(HTML_PAGE);
}

void loop() 
{
  static auto millsPrev = millis();
  auto mills = millis();
  auto diff = mills - millsPrev;

  gCtx->getLedController().tick(diff);
  gCtx->getWiFiController().tick(diff);
  gCtx->getWebServer().tick(diff);

  millsPrev = mills;
}
