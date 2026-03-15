#include "hardware.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

void setupScreen(){
    Wire.begin(OLED_SDA, OLED_SCL); 

    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false)){
        Serial.println(F("Screen Failed to init"));
        while(true);
    }

    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(1);
    display.setCursor(5, 5);
    display.println(F("Home Security System"));
    
    display.setTextSize(1);
    display.setCursor(35, 30);
    display.println(F("Booting..."));
    
    // Simple Loading Bar Animation
    int barWidth = 100;
    int barHeight = 8;
    int barX = (SCREEN_WIDTH - barWidth) / 2;
    int barY = 45;
    for(int i = 0; i <= barWidth; i += 5){
        // Draw the outline of the bar
        display.drawRect(barX, barY, barWidth, barHeight, SSD1306_WHITE);
        
        // Fill the bar based on progress
        display.fillRect(barX, barY, i, barHeight, SSD1306_WHITE);
        
        display.display();
        delay(100); // Adjust speed of the "boot" here
    }
}

void updateScreen(){
    display.clearDisplay();
    display.setTextColor(SSD1306_WHITE);

    // --- Header: WiFi & Server Status ---
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print(getWiFiStatus() ? F("WIFI: ON") : F("WIFI: OFF"));
    
    // Server status on the right side
    display.setCursor(80, 0);
    display.print(getServerStatus() ? F("SRV: OK") : F("SRV: !"));
    
    // Header underline
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    // --- Main Body: Door Status ---
    display.setTextSize(2);
    display.setCursor(30, 23);
    if(getDoorStatus()){
        display.println(F("OPEN!!"));
    } else {
        display.println(F("CLOSED"));
    }

    // --- Footer: Battery Info ---
    display.drawLine(0, 50, 128, 50, SSD1306_WHITE);
    display.setTextSize(1);
    display.setCursor(0, 55);
    display.print(F("BAT: "));
    display.print(getBatteryPercent());
    display.print(F("%"));

    display.setCursor(85, 55);
    display.print(getBatteryVoltage());
    display.print(F("V"));

    display.display();
}

bool getDoorStatus(){
    int NCCircuitComplete = digitalRead(PIN_REED_SWITCH_NC);
    return (NCCircuitComplete == HIGH);
}

// TODO
bool getWiFiStatus(){
    return true;
}

// TODO
bool getServerStatus(){
    return true;
}

int getBatteryPercent() {
    float v = getBatteryVoltage();

    // Look-Up Table for 3.7V LiPo
    // Format: {Voltage, Percentage}
    struct VoltageMap{ 
        float voltage; 
        int percentage; 
    };

    const VoltageMap lut[] = {
        {4.15, 100}, {4.05, 90}, {3.96, 80}, {3.89, 70},
        {3.82, 60},  {3.75, 50}, {3.69, 40}, {3.62, 30},
        {3.55, 20},  {3.48, 10}, {3.30, 5},  {3.20, 0}
    };

    if(v >= lut[0].voltage){
        return 100;
    }
    if(v <= lut[11].voltage){
        return 0;
    } 

    // Search the table and interpolate
    for(int i = 0; i < 11; i++){
        if(v <= lut[i].voltage && v > lut[i+1].voltage){
            // Linear interpolation between two points for extra smoothness
            return lut[i+1].percentage + (int)((v - lut[i+1].voltage) * (lut[i].percentage - lut[i+1].percentage) / (lut[i].voltage - lut[i+1].voltage));
        }
    }

    return 0;
}

float getBatteryVoltage() {
    uint32_t raw = 0;
    for(int i=0; i<20; i++) {
        raw += analogRead(A7);
        delay(1); 
    }
    float avgRaw = (float)raw / 20.0;
    
    // (Raw / Max ADC) * Logic Voltage * Divider Factor
    return (avgRaw / 4095.0) * 3.3 * 2.0 * 1.1;
}