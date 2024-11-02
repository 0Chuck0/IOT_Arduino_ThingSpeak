const channelID = '2717430  ';
const readAPIKey = 'TWYWVWMKXN6KKVYB';

async function fetchData(fieldNo) {
  try {
    // Fetch the latest entry with all fields
    const response = await fetch(`https://api.thingspeak.com/channels/${channelID}/fields/${fieldNo}/last.json?api_key=${readAPIKey}`);
    const data = await response.json();

    console.log(data);
    // Update the humidity and temperature values
    if (fieldNo == 1) {
      const humidity = parseFloat(data.field1);
      document.getElementById("humidityValue").innerText = isNaN(humidity) ? "N/A" : humidity.toFixed(2);
    }
    else if (fieldNo == 2) {
      const temp = parseFloat(data.field2);
      document.getElementById("temperatureValue").innerText = isNaN(temp) ? "N/A" : temp.toFixed(2);
    }
  } catch (error) {
    console.error("Error fetching data:", error);
    if (fieldNo == 1)
      document.getElementById("humidityValue").innerText = "Error";
    else if (fieldNo == 2)
      document.getElementById("temperatureValue").innerText = "Error";
  }
}

// Fetch data every 15 seconds
setInterval(() => fetchData(1), 15000);
setInterval(() => fetchData(2), 15000);
fetchData(1);
fetchData(2);
