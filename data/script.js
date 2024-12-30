function changeStation(increase) {
    fetch(`/change_station?increase=${increase}`)
        .then(response => response.json())
        .then(data => {
            document.getElementById('frequency').innerText = data.frequency;
        });
}

function adjustVolume(increase) {
    fetch(`/adjust_volume?increase=${increase}`)
        .then(response => response.json())
        .then(data => {
            document.getElementById('volume').innerText = data.volume;
        });
}

function fetchRDSInfo() {
    fetch('/rds')
        .then(response => response.json())
        .then(data => {
            document.getElementById('rds-info').innerText = data.info;
        });
}

function fetchWeather() {
    fetch('/weather')
        .then(response => response.json())
        .then(data => {
            document.getElementById('weather-info').innerText = data.weather;
        });
}

function connectToInternetRadio() {
    fetch('/connect')
        .then(response => response.json())
        .then(data => {
            alert(data.message);
        });
}

function fetchSignalStrength() {
    fetch('/get_signal_strength')
        .then(response => response.json())
        .then(data => {
            document.getElementById('signal-strength').innerText = data.signalStrength;
        });
}

function setFrequency() {
    const frequency = document.getElementById('frequency-input').value;
    fetch(`/set_frequency?frequency=${frequency}`)
        .then(response => response.json())
        .then(data => {
            document.getElementById('frequency').innerText = data.frequency;
        });
}

function setStationFrequency(frequency) {
    fetch(`/set_frequency?frequency=${frequency}`)
        .then(response => response.json())
        .then(data => {
            document.getElementById('frequency').innerText = data.frequency;
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