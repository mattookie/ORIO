#include <WiFi.h>
#include <WebServer.h>

// Network Details
const char* ssid = "mattphone";
const char* password = "tanisfat";

WebServer server(80); // Creates webserver on port 80

// HTML Content which will allow us to controll the star tracker
const char* HTML_CONTENT = R"rawliteral(
<!DOCTYPE html>

<!-- This contains the meta data about the document -->
<head>
    <meta charset = "UTF-8">

    <!-- This makes the website viewable on a mobile screen opposed to a desktop -->
    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <title> ORIO Star Tracker</title>

    <!-- Load Tailwind CSS For ease of esign -->
    <script src="https://cdn.tailwindcss.com"></script>
    <style>
        /* Mobile aesthetics */
        html, body {
            height: 100%;
            margin: 0;
            font-family: 'Inter', sans-serif;
            background-color: #030712; /* Dark Color Background */
            color: #d1d5db; /* Text Color */
        }

        /* Main Button Styling */
        .control-button:active {
            /* Will shift button down 1 when pressed */
            transform: translateY(1px);
            /* Removes shadow when pressed */
            box-shadow: none;
        }

    </style>
</head>
<body class="flex flex-col items-center justify-start p-4 md:p-8">

    <!-- Title Code -->
    <header class="w-full max-w-lg mb-6 p-4 bg-gray-800 rounded-xl shadow-lg border-t-2 border-indigo-500">
            <h1 class="text-3xl font-extrabold text-white text-center">ORIO STAR TRACKER</h1>
            <div id="statusMessage" class="mt-2 text-center text-sm font-medium h-6 text-gray-400">
                Main Menu
            </div>
        </header>

    <!-- Back button, not visible on main page but visible else where -->
    <!-- The back button mechanics will be programmed later, we are just establishing it here -->
    <div id="backButtonContainer" class="w-full max-w-lg mb-4 hidden">
        <button id="backButton" class="w-full py-2 bg-gray-700 hover:bg-gray-600 rounded-lg text-white font-semibold flex items-center justify-center">
            <svg xmlns="http://www.w3.org/2000/svg" class="h-5 w-5 mr-2" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2"><path stroke-linecap="round" stroke-linejoin="round" d="M10 19l-7-7m0 0l7-7m-7 7h18" /></svg>
            Back to Main Menu
        </button>
    </div>

    <!-- Code for the main menu -->
    <main id="viewContainer" class="w-full max-w-lg flex-grow">
        
        <!-- 1. main menu view -->
        <section id="MAIN_VIEW" class="p-4 bg-gray-800 rounded-xl shadow-lg border-b-4 border-indigo-600">
            <h2 class="text-2xl font-bold mb-4 text-center text-indigo-400">Select Operation Mode</h2>
            <div class="space-y-4">
                <button data-target-view="GOTO_VIEW" class="view-nav-button w-full py-4 bg-[#13576E] hover:bg-[#0e4152] text-white text-xl font-bold rounded-lg control-button">
                    GoTo
                </button>
                <button data-target-view="SETUP_VIEW" class="view-nav-button w-full py-4 bg-[#13576E] hover:bg-[#0e4152] text-white text-xl font-bold rounded-lg control-button">
                    Setup Control
                </button>
                <button data-target-view="TRACK_VIEW" class="view-nav-button w-full py-4 bg-[#13576E] hover:bg-[#0e4152] text-white text-xl font-bold rounded-lg control-button">
                    Tracking
                </button>
            </div>
        </section>

        <!-- 2. goto view -->
        <section id="GOTO_VIEW" class="hidden p-4 bg-gray-800 rounded-xl shadow-lg border-b-4 border-blue-600">
            <h2 class="text-2xl font-bold mb-4 text-blue-400">GoTo Target Coordinates</h2>
            
            <div class="space-y-4 mb-6">
                <!-- RA Input -->
                <div>
                    <label for="raInput" class="block text-sm font-medium text-gray-300 mb-1">Right Ascension (RA) - HH:MM:SS</label>
                    <input type="text" id="raInput" value="00:00:00" placeholder="HH:MM:SS" class="w-full p-3 bg-gray-700 border border-gray-600 rounded-lg text-white focus:ring-blue-500 focus:border-blue-500">
                </div>
                
                <!-- DEC Input -->
                <div>
                    <label for="decInput" class="block text-sm font-medium text-gray-300 mb-1">Declination (DEC) - +/- DD:MM:SS</label>
                    <input type="text" id="decInput" value="+00:00:00" placeholder="+/- DD:MM:SS" class="w-full p-3 bg-gray-700 border border-gray-600 rounded-lg text-white focus:ring-blue-500 focus:border-blue-500">
                </div>
            </div>
            
            <button id="goToButton" class="w-full py-4 bg-blue-600 hover:bg-blue-700 text-white text-xl font-bold rounded-lg control-button">
                INITIATE GOTO
            </button>
        </section>

        <!-- 3. setup view (manual motor controls) -->
        <section id="SETUP_VIEW" class="hidden">
             <!-- Speed Control Slider -->
            <div class="w-full max-w-lg mb-8">
                <h2 class="text-lg font-bold mb-2 text-indigo-300">Slew/Jog Speed (<span id="speedValue">10x</span>)</h2>
                <input type="range" id="speedSlider" min="1" max="100" value="10" step="1" 
                       class="w-full h-2 bg-gray-700 rounded-lg appearance-none cursor-pointer range-lg"
                       style="accent-color: #6366f1;">
            </div>

            <!-- Right Ascension (RA) Controls -->
            <div class="w-full max-w-lg mb-8 p-4 bg-gray-800 rounded-xl shadow-lg border-l-2 border-green-500">
                <h2 class="text-xl font-bold mb-3 text-green-300">Right Ascension (RA)</h2>
                <div class="grid grid-cols-3 gap-3">
                    <div class="col-span-1"></div>
                    <button data-axis="RA" data-direction="UP" class="control-button bg-green-600 hover:bg-green-700 text-white font-semibold py-3 rounded-lg flex items-center justify-center">
                        <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6 transform rotate-90" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2">
                            <path stroke-linecap="round" stroke-linejoin="round" d="M9 5l7 7-7 7" />
                        </svg>
                        Clockwise
                    </button>
                    <div class="col-span-1"></div>
                    
                    <button data-axis="RA" data-direction="LEFT" class="control-button bg-green-500 hover:bg-green-600 text-white font-semibold py-3 rounded-lg flex items-center justify-center">
                        <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6 transform rotate-180" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2">
                            <path stroke-linecap="round" stroke-linejoin="round" d="M9 5l7 7-7 7" />
                        </svg>
                        West
                    </button>
                    
                    <button data-axis="RA" data-direction="STOP" class="control-button bg-gray-600 hover:bg-gray-700 text-white font-semibold py-3 rounded-lg">
                        STOP
                    </button>
                    
                    <button data-axis="RA" data-direction="RIGHT" class="control-button bg-green-500 hover:bg-green-600 text-white font-semibold py-3 rounded-lg flex items-center justify-center">
                        East
                        <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2">
                            <path stroke-linecap="round" stroke-linejoin="round" d="M9 5l7 7-7 7" />
                        </svg>
                    </button>
                    
                    <div class="col-span-1"></div>
                    <button data-axis="RA" data-direction="DOWN" class="control-button bg-green-600 hover:bg-green-700 text-white font-semibold py-3 rounded-lg flex items-center justify-center">
                        <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6 transform -rotate-90" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2">
                            <path stroke-linecap="round" stroke-linejoin="round" d="M9 5l7 7-7 7" />
                        </svg>
                        Counter-CW
                    </button>
                    <div class="col-span-1"></div>
                </div>
            </div>

            <!-- Declination (DEC) Controls -->
            <div class="w-full max-w-lg p-4 bg-gray-800 rounded-xl shadow-lg border-r-2 border-sky-500">
                <h2 class="text-xl font-bold mb-3 text-sky-300">Declination (DEC)</h2>
                <div class="grid grid-cols-3 gap-3">
                    <div class="col-span-1"></div>
                    <button data-axis="DEC" data-direction="UP" class="control-button bg-sky-600 hover:bg-sky-700 text-white font-semibold py-3 rounded-lg">
                        North (+)
                    </button>
                    <div class="col-span-1"></div>
                    
                    <button data-axis="DEC" data-direction="LEFT" class="control-button bg-gray-400 text-gray-800 font-semibold py-3 rounded-lg opacity-50 cursor-not-allowed" disabled>
                        N/A
                    </button>
                    
                    <button data-axis="DEC" data-direction="STOP" class="control-button bg-gray-600 hover:bg-gray-700 text-white font-semibold py-3 rounded-lg">
                        STOP
                    </button>
                    
                    <button data-axis="DEC" data-direction="RIGHT" class="control-button bg-gray-400 text-gray-800 font-semibold py-3 rounded-lg opacity-50 cursor-not-allowed" disabled>
                        N/A
                    </button>
                    
                    <div class="col-span-1"></div>
                    <button data-axis="DEC" data-direction="DOWN" class="control-button bg-sky-600 hover:bg-sky-700 text-white font-semibold py-3 rounded-lg">
                        South (-)
                    </button>
                    <div class="col-span-1"></div>
                </div>
            </div>
        </section>
        
        <!-- 4. TRACK VIEW -->
        <section id="TRACK_VIEW" class="hidden p-4 bg-gray-800 rounded-xl shadow-lg border-b-4 border-yellow-600">
            <h2 class="text-2xl font-bold mb-4 text-yellow-400">Automatic Tracking Control</h2>
            
            <p class="text-gray-300 mb-6">Toggling this will command the RA motor to start moving at **sidereal rate** to compensate for the Earth's rotation.</p>

            <div class="flex items-center justify-between p-4 bg-gray-700 rounded-xl shadow-inner">
                <label for="trackingModeToggle" class="text-xl font-semibold text-white">Tracking Status:</label>
                <button id="trackingModeToggle" 
                        class="px-5 py-2 font-bold rounded-full text-sm transition-colors duration-300 bg-red-600 text-white hover:bg-red-700" 
                        data-mode="STOPPED">
                    STOPPED
                </button>
            </div>
            
            <div class="mt-4 p-3 bg-gray-700 rounded-lg text-sm text-gray-400">
                <p>Ensure your polar alignment is complete before starting long exposures.</p>
            </div>
        </section>

    </main>

    <div class="mt-8 text-center text-xs text-gray-500 max-w-lg">
        
        <p>T.U.R.T.L.E. Robotics ORIO Project Team</p>
    </div>

    <!-- The below describes the star tracker controller class which manages the connection with ESP32 -->

    <script>
        // CONSTANTS (Creates a dictionary to refer to the previous ID's of the different page views for ease of programming)
        const VIEWS = {
            MAIN: 'MAIN_VIEW',
            GOTO: 'GOTO_VIEW',
            SETUP: 'SETUP_VIEW',
            TRACK: 'TRACK_VIEW'
        };
        
        // Here change the IP Address of the ESP32
        const ESP32_IP = 'http://192.168.4.1'; 

        // Classes (This is a javascript defined class because it will serve to function on the website and will send stuff
        // back to the ESP32 where it will take this information and run it with the C++ Code)
        
        /**
         * Class: StarTrackerController
         * Manages device state and communication with ESP32
         */
        class StarTrackerController {
            constructor() {
                this.isTracking = false;
                this.status = '';
            }
            
            /**
             * Primary function to send commands to the ESP32/Stepper Motors.
             */
            async sendCommand(command, value) {
                const url = `${ESP32_IP}/control?cmd=${command}&val=${value}`;

                try {
                    const response = await fetch(url, { method: 'GET' });
                    
                    if (!response.ok) {
                        throw new Error(`HTTP Error: ${response.status}`);
                    }
                    
                    const responseText = await response.text();
                    this.status = `Sent ${command}. Server Response: ${responseText}`;
                    return this.status;

                } catch (error) {
                    this.status = `COMMUNICATION ERROR: Could not reach tracker at ${ESP32_IP}. Check Wi-Fi. (${error.message})`;
                    return this.status;
                }
            }

            // Methods (Getters/Setters)

            async toggleTracking(isTracking) {
                this.isTracking = isTracking;
                const command = isTracking ? 'TRACK_START' : 'TRACK_STOP';
                const value = 'SIDEREAL'; 
                return this.sendCommand(command, value);
            }

            async jogAxis(axis, direction) {
                if (direction === 'STOP') {
                    return this.sendCommand('STOP', axis); // Command: STOP, Value: RA or DEC
                }
                const command = 'JOG';
                // Value format: RA_UP_10 (Axis_Direction_Speed)
                const value = `${axis}_${direction}_${this.speedMultiplier}`; 
                return this.sendCommand(command, value);
            }
            
            async goTo(ra, dec) {
                const command = 'GOTO';
                // Value format: 00:00:00,+00:00:00 (RA,DEC)
                const value = `${ra},${dec}`;
                return this.sendCommand(command, value);
            }
        }

)rawliteral";