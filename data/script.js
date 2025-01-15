// radio control functions
function toggleRadio() {
    fetch('/toggleRadio')
        .then(response => response.text())
        .then(data => {
            console.log(data);
            alert(data);
            updateRadioStatus();
        });
}

function updateRadioStatus() {
    fetch('/getRadioStatus')
        .then(response => response.text())
        .then(data => {
            document.getElementById('radio-status').innerText = data;
        });
}

function fetchSignalStrength() {
    fetch('/getSignalStrength')
        .then(response => response.text())
        .then(data => {
            document.getElementById('signal-level').innerText = data;
        });
}

function changeStation(increase) {
    const endpoint = increase ? '/increaseFrequency' : '/decreaseFrequency';
    fetch(endpoint)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentFrequency();
            fetchSignalStrength();
        });
}

function fetchCurrentFrequency() {
    fetch('/getFrequency')
        .then(response => response.text())
        .then(data => {
            document.getElementById('frequency').innerText = data;
        });
}

function setFrequency() {
    const frequency = document.getElementById('frequency-input').value;
    if (!frequency) {
        alert('Please enter a frequency.');
        return;
    }
    fetch(`/setFrequency?freq=${frequency}`)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentFrequency();
            fetchSignalStrength();
        });
}

function setStationFrequency(frequency) {
    fetch(`/setFrequency?freq=${frequency}`)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentFrequency();
            fetchSignalStrength();
        });
}

function searchRadioStations() {
    fetch('/searchRadioStations')
        .then(response => response.text())
        .then(data => {
            console.log(data);
            alert('Radio stations search completed.');
            fetchRadioStations();
        });
}

function saveRadioStations() {
    fetch('/saveRadioStations')
        .then(response => response.text())
        .then(data => {
            console.log(data);
            alert('Radio stations saved.');
            fetchSavedRadioStations();
        });
}

function fetchRadioStations() {
    fetch('/getRadioStations')
        .then(response => response.json())
        .then(data => {
            const stationsList = document.getElementById('radio-stations-list');
            stationsList.innerHTML = '';
            data.forEach(station => {
                const listItem = document.createElement('li');
                listItem.innerHTML = `${station.frequency} MHz <button onclick="setStationFrequency(${station.frequency})">Listen</button>`;
                stationsList.appendChild(listItem);
            });
        });
}

function fetchSavedRadioStations() {
    fetch('/getSavedRadioStations')
        .then(response => response.json())
        .then(data => {
            const savedStationsList = document.getElementById('saved-stations-list');
            savedStationsList.innerHTML = '';
            data.forEach(station => {
                const listItem = document.createElement('li');
                listItem.innerHTML = `${station.frequency} MHz <button onclick="setStationFrequency(${station.frequency})">Listen</button>`;
                savedStationsList.appendChild(listItem);
            });
        });
}
// bluetooth control functions
function toggleBluetooth() {
    fetch('/toggleBluetooth')
        .then(response => response.text())
        .then(data => {
            console.log(data);
            alert(data);
        });
}

function updateBluetoothStatus() {
    fetch('/getBluetoothStatus')
        .then(response => response.text())
        .then(data => {
            document.getElementById('bluetooth-status').innerText = data;
        });
}

function fetchNowPlaying() {
    fetch('/getNowPlaying')
        .then(response => response.json())
        .then(data => {
            document.getElementById('current-title').innerText = data.title;
            document.getElementById('current-artist').innerText = data.artist;
            document.getElementById('current-album').innerText = data.album;
        });
}

function fetchCurrentVolume() {
    fetch('/getVolume')
        .then(response => response.text())
        .then(data => {
            document.getElementById('volume').innerText = data;
        });
}

function adjustVolume(increase) {
    const endpoint = increase ? '/volumeUp' : '/volumeDown';
    fetch(endpoint)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentVolume();
        });
}

function volumeUp() {
    fetch('/volumeUp')
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentVolume();
        });
}

function volumeDown() {
    fetch('/volumeDown')
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentVolume();
        });
}

function playNextTrack() {
    fetch('/playNextTrack')
        .then(response => response.text())
        .then(data => {
            console.log(data);
        });
}

function playPreviousTrack() {
    fetch('/playPreviousTrack')
        .then(response => response.text())
        .then(data => {
            console.log(data);
        });
}

function togglePlayback() {
    fetch('/togglePlayback')
        .then(response => response.text())
        .then(data => {
            console.log(data);
        });
}

// system control functions
function restartESP() {
    fetch('/restart')
        .then(response => {
            if (response.ok) {
                alert('ESP32 is restarting...');
            } else {
                alert('Failed to restart ESP32');
            }
        })
        .catch(error => {
            console.error('Error:', error);
            alert('Failed to restart ESP32');
        });
}

function fetchSystemStats() {
    fetch('/getSystemStats')
        .then(response => response.json())
        .then(data => {
            const statsElement = document.getElementById('system-stats');
            statsElement.innerHTML = `
                <p>Total Heap: ${data.totalHeap} KB</p>
                <p>Free Heap: ${data.freeHeap} KB</p>
                <p>Used Heap: ${data.usedHeap} KB</p>
                <p>Free PSRAM: ${data.freePSRAM} KB</p>
                <p>Free SPIFFS: ${data.freeSPIFFS} KB</p>
                <p>Chip Temperature: ${data.chipTemp} °C</p>
            `;
        });
}

// wifi control functions
function fetchWiFiDetails() {
    fetch('/getWiFiDetails')
        .then(response => response.json())
        .then(data => {
            document.getElementById('current-ssid').innerText = data.ssid;
            document.getElementById('current-ip').innerText = data.ip;
            document.getElementById('current-signal').innerText = data.signalStrength;
        });
}

function scanWiFiNetworks() {
    fetch('/scanWiFiNetworks')
        .then(response => response.json())
        .then(data => {
            const networksList = document.getElementById('networks-list');
            networksList.innerHTML = '';
            data.forEach(network => {
                const listItem = document.createElement('li');
                listItem.innerHTML = `${network.ssid} (${network.rssi} dBm) <button onclick="selectNetwork('${network.ssid}')">Select</button>`;
                networksList.appendChild(listItem);
            });
        });
}

function selectNetwork(ssid) {
    document.getElementById('wifi-ssid').value = ssid;
    document.getElementById('wifi-password').value = '';
    alert(`Selected network: ${ssid}. Please enter the password if required.`);
}

function connectToWiFi() {
    const ssid = document.getElementById('wifi-ssid').value;
    const password = document.getElementById('wifi-password').value;
    if (!ssid) {
        alert('Please enter an SSID.');
        return;
    }
    fetch(`/connectToWiFi?ssid=${encodeURIComponent(ssid)}&password=${encodeURIComponent(password)}`)
        .then(response => response.text())
        .then(data => {
            alert(data);
            fetchWiFiDetails();
        });
}

function addWiFiCredentials() {
    const ssid = document.getElementById('wifi-ssid').value;
    const password = document.getElementById('wifi-password').value;
    if (!ssid) {
        alert('Please enter an SSID.');
        return;
    }
    fetch(`/addWiFiCredentials?ssid=${encodeURIComponent(ssid)}&password=${encodeURIComponent(password)}`)
        .then(response => response.text())
        .then(data => {
            alert(data);
            fetchSavedWiFiCredentials();
        });
}

function removeWiFiCredentials(ssid) {
    fetch(`/removeWiFiCredentials?ssid=${encodeURIComponent(ssid)}`)
        .then(response => response.text())
        .then(data => {
            alert(data);
            fetchSavedWiFiCredentials();
        });
}

function clearWiFiCredentials() {
    fetch('/clearWiFiCredentials')
        .then(response => response.text())
        .then(data => {
            alert(data);
            fetchSavedWiFiCredentials();
        });
}

function fetchSavedWiFiCredentials() {
    fetch('/getSavedWiFiCredentials')
        .then(response => response.json())
        .then(data => {
            const savedNetworksList = document.getElementById('saved-networks-list');
            savedNetworksList.innerHTML = '';
            data.forEach(network => {
                const listItem = document.createElement('li');
                listItem.innerHTML = `${network.ssid} (ID: ${network.id}) <button onclick="connectToSavedWiFi('${network.ssid}')">Connect</button> <button onclick="removeWiFiCredentials('${network.ssid}')">Remove</button>`;
                savedNetworksList.appendChild(listItem);
            });
        });
}

function connectToSavedWiFi(ssid) {
    fetch(`/connectToWiFi?ssid=${encodeURIComponent(ssid)}&password=`)
        .then(response => response.text())
        .then(data => {
            alert(data);
            fetchWiFiDetails();
        });
}

function startAP() {
    const ssid = document.getElementById('ap-ssid').value;
    const password = document.getElementById('ap-password').value;
    if (!ssid) {
        alert('Please enter an SSID for the Access Point.');
        return;
    }
    fetch(`/startAP?ssid=${encodeURIComponent(ssid)}&password=${encodeURIComponent(password)}`)
        .then(response => response.text())
        .then(data => {
            alert(data);
            fetchAPInfo();
        });
}

function stopAP() {
    fetch('/stopAP')
        .then(response => response.text())
        .then(data => {
            alert(data);
            fetchAPInfo();
        });
}

function fetchAPInfo() {
    fetch('/printAPInfo')
        .then(response => response.text())
        .then(data => {
            document.getElementById('ap-info').innerText = data;
        });
}

function printConnectedDevices() {
    fetch('/printConnectedDevices')
        .then(response => response.text())
        .then(data => {
            document.getElementById('connected-devices').innerText = data;
        });
}

function fetchRDSInfo() {
    fetch('/getRDSInfo')
        .then(response => response.text())
        .then(data => {
            document.getElementById('rds-info').innerText = data;
        });
}

function fetchWeather() {
    fetch('/getWeather')
        .then(response => response.text())
        .then(data => {
            document.getElementById('weather-info').innerText = data;
        });
}

function connectToInternetRadio() {
    fetch('/connectToInternetRadio')
        .then(response => response.text())
        .then(data => {
            alert(data);
        });
}

function startSystemStatsUpdate() {
    fetchSystemStats();
    setInterval(fetchSystemStats, 1000); // update every second
}

// js things
document.addEventListener('DOMContentLoaded', (event) => {
    updateRadioStatus();
    fetchCurrentFrequency();
    fetchSignalStrength();
    fetchRadioStations();
    fetchNowPlaying();
    fetchCurrentVolume();
    startSystemStatsUpdate();
    fetchWiFiDetails();
    fetchSavedWiFiCredentials();
    fetchAPInfo();
    updateBluetoothStatus();
});

function showSection(sectionId) {
    const sections = document.querySelectorAll('.section');
    sections.forEach(section => {
        section.style.display = 'none';
    });
    document.getElementById(sectionId).style.display = 'block';
}

function toggleDarkMode() {
    document.body.classList.toggle('dark-mode');
}