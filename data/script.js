function toggleRadio() {
    fetch('/toggleRadio')
        .then(response => response.text())
        .then(data => {
            console.log(data);
        });
}

function toggleBluetooth() {
    fetch('/toggleBluetooth')
        .then(response => response.text())
        .then(data => {
            console.log(data);
        });
}

function changeStation(increase) {
    const endpoint = increase ? '/increaseFrequency' : '/decreaseFrequency';
    fetch(endpoint)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentFrequency();
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

function fetchCurrentFrequency() {
    fetch('/getFrequency')
        .then(response => response.text())
        .then(data => {
            document.getElementById('frequency').innerText = data;
        });
}

function fetchCurrentVolume() {
    fetch('/getVolume')
        .then(response => response.text())
        .then(data => {
            document.getElementById('volume').innerText = data;
        });
}

function setFrequency() {
    const frequency = document.getElementById('frequency-input').value;
    fetch(`/setFrequency?freq=${frequency}`)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentFrequency();
        });
}

function setStationFrequency(frequency) {
    fetch(`/setFrequency?freq=${frequency}`)
        .then(response => response.text())
        .then(data => {
            console.log(data);
            fetchCurrentFrequency();
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
                listItem.textContent = `${network.ssid} (${network.rssi} dBm)`;
                networksList.appendChild(listItem);
            });
        });
}

function connectToWiFi() {
    const ssid = document.getElementById('wifi-ssid').value;
    const password = document.getElementById('wifi-password').value;
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
    fetch(`/addWiFiCredentials?ssid=${encodeURIComponent(ssid)}&password=${encodeURIComponent(password)}`)
        .then(response => response.text())
        .then(data => {
            alert(data);
            fetchSavedWiFiCredentials();
        });
}

function removeWiFiCredentials() {
    const ssid = document.getElementById('wifi-ssid').value;
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
                listItem.textContent = `${network.ssid} (ID: ${network.id})`;
                savedNetworksList.appendChild(listItem);
            });
        });
}

function startSystemStatsUpdate() {
    fetchSystemStats();
    setInterval(fetchSystemStats, 1000); // update every second
}

document.addEventListener('DOMContentLoaded', (event) => {
    fetchCurrentFrequency();
    fetchCurrentVolume();
    startSystemStatsUpdate();
    fetchWiFiDetails();
    fetchSavedWiFiCredentials();
});