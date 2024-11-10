const channelID = '2717430';
const readAPIKey = 'TWYWVWMKXN6KKVYB';
const writeAPIKey = '5KSKJ52T5SAV89DU';

async function readLED() {
  try {
    // Fetch the last entry for field 3 from ThingSpeak
    const response = await fetch(`https://api.thingspeak.com/channels/${channelID}/fields/3/last.json?api_key=${readAPIKey}`);
    
    // Check if the response is successful
    if (!response.ok) {
      throw new Error(`Network response was not ok: ${response.statusText}`);
    }
    
    // Parse the JSON response
    const data = await response.json();
    
    // Extract the LED status value from field 3 and return it
    const ledStatus = parseInt(data.field3, 10); // Convert to integer
    const ledButton = document.getElementById("led_btn");
    const ledLabel = document.getElementById("led_stat");

    // Update the button and label based on LED status
    if (ledStatus === 1) {
      ledButton.textContent = "OFF";
      ledButton.className = "styled-button1 turn_off";
      ledLabel.textContent = "The LED is ON";
    } else if (ledStatus === 0) {
      ledButton.textContent = "ON";
      ledButton.className = "styled-button1 turn_on";
      ledLabel.textContent = "The LED is OFF";
    } else {
      console.log("Unexpected LED status value:", ledStatus);
      ledButton.textContent = "Error";
    }

    return ledStatus; // Ensure the status is returned

  } catch (error) {
    console.error("Failed to fetch LED status:", error);
    return null;
  }
}

async function setLED() {
  try {
    // Get the current LED status
    const currentStatus = await readLED();
    
    if (currentStatus === null) {
      console.error("Unable to read LED status.");
      return;
    }

    // Determine the new status to send (toggle between 1 and 0)
    const newStatus = currentStatus === 1 ? 0 : 1;

    // Update ThingSpeak with the new LED status
    const updateResponse = await fetch(
      `https://api.thingspeak.com/update?api_key=${writeAPIKey}&field3=${newStatus}`
    );

    if (!updateResponse.ok) {
      throw new Error(`Failed to update LED status: ${updateResponse.statusText}`);
    }

    // Optimistically update button content and label immediately
    const ledButton = document.getElementById("led_btn");
    const ledLabel = document.getElementById("led_stat");
    ledButton.textContent = newStatus === 1 ? "OFF" : "ON";
    ledButton.className = newStatus === 1 ? "styled-button1 turn_off":"styled-button1 turn_on";
    ledLabel.textContent = newStatus === 1 ? "The LED is ON" : "The LED is OFF";

    // Optionally re-read to confirm status after a delay
    // setTimeout(() => {readLED(); }, 5000);

  } catch (error) {
    console.error("Failed to set LED status:", error);
  }
}


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

//add event handler
document.getElementById('led_btn').onclick = setLED;
setInterval(() => fetchData(1), 30000);
setInterval(() => fetchData(2), 30000);
fetchData(1);
fetchData(2);
readLED();