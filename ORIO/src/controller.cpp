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

    <!-- Load Tailwind CSS For ease of design and pretty -->
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
        <section id="GOTO_VIEW" class="hidden p-4 bg-gray-800 rounded-xl shadow-lg border-b-4 border-[#703764]">
            <h2 class="text-2xl font-bold mb-4 text">GoTo</h2>
            
            <div class="space-y-4 mb-6">
                <!-- Here there will be different buttons for different celestial objects we plan to track -->
                <div>
                    <button id="goto" class="w-full py-4 bg-[#703764] hover:bg-[#54294b] text-white text-xl font-bold rounded-lg control-button">
                    Vega
                    </button>
                </div>
                
                <!-- Another Object -->
                <div>
                    <button id="Moon" class="w-full py-4 bg-[#703764] hover:bg-[#54294b] text-white text-xl font-bold rounded-lg control-button">
                    Moon
                    </button>
                </div>
                <!-- Another Object -->
                <div>
                    <button id="Running Man Cluster" class="w-full py-4 bg-[#703764] hover:bg-[#54294b] text-white text-xl font-bold rounded-lg control-button">
                    Running Man Cluster
                    </button>
                </div>
                <!-- Another Object -->
                <div>
                    <button id="Saturn" class="w-full py-4 bg-[#703764] hover:bg-[#54294b] text-white text-xl font-bold rounded-lg control-button">
                    Saturn
                    </button>
                </div>
                <!-- Another Object -->
                <div>
                    <button id="Starfish Cluster" class="w-full py-4 bg-[#703764] hover:bg-[#54294b] text-white text-xl font-bold rounded-lg control-button">
                    Starfish Cluster
                    </button>
                </div>
                <!-- Another Object -->
                <div>
                    <button id="Jellyfish Nebula" class="w-full py-4 bg-[#703764] hover:bg-[#54294b] text-white text-xl font-bold rounded-lg control-button">
                    Jellyfish Nebula
                    </button>
                </div>
                <!-- Another Object -->
                <div>
                    <button id="Mars" class="w-full py-4 bg-[#703764] hover:bg-[#54294b] text-white text-xl font-bold rounded-lg control-button">
                    Mars
                    </button>
                </div>
            </div>
        </section>

        <!-- 3. setup view (manual motor controls) -->
        <section id="SETUP_VIEW" class="hidden">
            
            <!-- Right Ascension (RA) Controls -->
            <div class="w-full max-w-lg mb-8 p-4 bg-gray-800 rounded-xl shadow-lg border-l-2 border-[#4682B4] shadow-rg border-r-2 border-[#4682B4]" >
                <h2 class="text-xl font-bold mb-3 text-[#4682B4]">Right Ascension (RA)</h2>
                <div class = "mt-6"></div>
                <div class="grid grid-cols-3 gap-3">
                    
                    <button data-axis="RA" data-direction="LEFT" class="control-button bg-[#4682B4] hover:bg-[#346187] text-white font-semibold py-3 rounded-lg flex items-center justify-center">
                        <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6 transform rotate-180" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2">
                            <path stroke-linecap="round" stroke-linejoin="round" d="M9 5l7 7-7 7" />
                        </svg>
                    </button>
                    
                    <button data-axis="RA" data-direction="STOP" class="control-button bg-gray-600 hover:bg-gray-700 text-white font-semibold py-3 rounded-lg">
                        STOP
                    </button>
                    
                    <button data-axis="RA" data-direction="RIGHT" class="control-button bg-[#4682B4] hover:bg-[#346187] text-white font-semibold py-3 rounded-lg flex items-center justify-center">
                        <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2">
                            <path stroke-linecap="round" stroke-linejoin="round" d="M9 5l7 7-7 7" />
                        </svg>
                    </button>
                    <div class = "mt-3"></div>
                </div>
            </div>

            <!-- Declination (DEC) Controls -->
            <div class="w-full max-w-lg mb-8 p-4 bg-gray-800 rounded-xl shadow-lg border-l-2 border-[#7649fe] shadow-rg border-r-2 border-[#7649fe]" >
                <h2 class="text-xl font-bold mb-3 text-[#7649fe]">Declination (DEC)</h2>
                <div class = "mt-6"></div>
                <div class="grid grid-cols-3 gap-3">
                    
                    <button data-axis="DEC" data-direction="LEFT" class="control-button bg-[#7649fe] hover:bg-[#3e01f4] text-white font-semibold py-3 rounded-lg flex items-center justify-center">
                        <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6 transform rotate-180" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2">
                            <path stroke-linecap="round" stroke-linejoin="round" d="M9 5l7 7-7 7" />
                        </svg>
                    </button>
                    
                    <button data-axis="DEC" data-direction="STOP" class="control-button bg-gray-600 hover:bg-gray-700 text-white font-semibold py-3 rounded-lg">
                        STOP
                    </button>
                    
                    <button data-axis="DEC" data-direction="RIGHT" class="control-button bg-[#7649fe] hover:bg-[#3e01f4] text-white font-semibold py-3 rounded-lg flex items-center justify-center">
                        <svg xmlns="http://www.w3.org/2000/svg" class="h-6 w-6" fill="none" viewBox="0 0 24 24" stroke="currentColor" stroke-width="2">
                            <path stroke-linecap="round" stroke-linejoin="round" d="M9 5l7 7-7 7" />
                        </svg>
                    </button>
                    <div class = "mt-3"></div>
                </div>
            </div>
        </section>
        
        <!-- 4. TRACK VIEW -->
        <section id="TRACK_VIEW" class="hidden p-4 bg-gray-800 rounded-xl shadow-lg border-b-4 border-yellow-600">
            <h2 class="text-2xl font-bold mb-4 text-yellow-400">Automatic Tracking</h2>
            
            <p class="text-gray-300 mb-6">Togglign this will start automatically tracking based on the GoTo provided.</p>

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
                // Value format: RA_UP_10
                const value = `${axis}_${direction}_${10}`; 
                return this.sendCommand(command, value);
            }
            
            async goTo(ra, dec) {
                const command = 'GOTO';
                // Value format: 00:00:00,+00:00:00 (RA,DEC)
                const value = `${ra},${dec}`;
                return this.sendCommand(command, value);
            }
        }
        
        /**
         * Class: UIHandler
         * Manages all DOM interactions, event listeners, and view switching.
         */
        class UIHandler {
            constructor(controller) {
                this.controller = controller;
                this.currentView = VIEWS.MAIN;
                
                // DOM Elements
                this.statusMessage = document.getElementById('statusMessage');
                this.viewContainer = document.getElementById('viewContainer');
                this.backButtonContainer = document.getElementById('backButtonContainer');
                
                // Setup/Jog Elements
                this.controlButtons = document.querySelectorAll('#SETUP_VIEW button[data-axis]');
                
                // Track Elements
                this.trackingToggle = document.getElementById('trackingModeToggle');

                // GoTo Elements
                this.raInput = document.getElementById('raInput');
                this.decInput = document.getElementById('decInput');


                this.setupEventListeners();
                this.renderView(VIEWS.MAIN);
            }

            setupEventListeners() {
                // 1. Navigation Buttons (Main Menu)
                document.querySelectorAll('.view-nav-button').forEach(button => {
                    button.addEventListener('click', (e) => {
                        this.renderView(e.currentTarget.dataset.targetView);
                    });
                });
                
                // 2. Back Button
                document.getElementById('backButton').addEventListener('click', () => {
                    this.renderView(VIEWS.MAIN);
                });

                // 3. Setup/Jog Controls
                this.controlButtons.forEach(button => {
                    button.addEventListener('mousedown', (e) => this.handleJogStart(e));
                    button.addEventListener('mouseup', (e) => this.handleJogStop(e));
                    button.addEventListener('touchstart', (e) => { e.preventDefault(); this.handleJogStart(e); });
                    button.addEventListener('touchend', (e) => { e.preventDefault(); this.handleJogStop(e); });
                });
                
                // 4. Tracking Mode Toggle
                this.trackingToggle.addEventListener('click', () => this.handleTrackingToggle());
                
                // 5. GoTo Button

            }

            // --- NAVIGATION LOGIC ---

            renderView(viewId) {
                this.currentView = viewId;
                
                // Hide all views
                Object.values(VIEWS).forEach(id => {
                    const el = document.getElementById(id);
                    if (el) el.classList.add('hidden');
                });

                // Show the target view
                const targetEl = document.getElementById(viewId);
                if (targetEl) {
                    targetEl.classList.remove('hidden');
                }
                
                // Manage Back Button visibility
                if (viewId === VIEWS.MAIN) {
                    this.backButtonContainer.classList.add('hidden');
                    this.updateStatus('Controller ready. Select a mode.');
                } else {
                    this.backButtonContainer.classList.remove('hidden');
                    this.updateStatus(`Mode: ${viewId.replace('_VIEW', '')}`);
                }
            }
            
            // --- HANDLERS ---

            async handleJogStart(e) {
                const button = e.currentTarget;
                const axis = button.dataset.axis;
                const direction = button.dataset.direction;
                
                // If the user clicks 'STOP', send the STOP command directly
                if (direction === 'STOP') {
                    await this.controller.jogAxis(axis, direction);
                    this.updateStatus(`Manual STOP command sent for ${axis}`);
                    return;
                } 
                
                // Otherwise, send the JOG command
                button.disabled = true;
                const status = await this.controller.jogAxis(axis, direction);
                button.disabled = false;
                this.updateStatus(status);
            }
            
            async handleJogStop(e) {
                const button = e.currentTarget;
                const axis = button.dataset.axis;
                const direction = button.dataset.direction;
                
                // We only want to send STOP if the button being released was NOT the dedicated STOP button
                if (direction !== 'STOP') {
                    const status = await this.controller.jogAxis(axis, 'STOP');
                    this.updateStatus(`STOP command automatically sent for ${axis}.`);
                }
            }

            async handleTrackingToggle() {
                const newMode = !this.controller.isTracking;
                
                this.trackingToggle.disabled = true; 
                
                const statusMessage = await this.controller.toggleTracking(newMode);
                
                this.trackingToggle.disabled = false;
                
                const modeText = newMode ? 'SIDEREAL TRACKING' : 'STOPPED';
                
                this.trackingToggle.textContent = modeText;
                
                // Update button color based on mode
                if (newMode) {
                    this.trackingToggle.classList.remove('bg-red-600', 'hover:bg-red-700');
                    this.trackingToggle.classList.add('bg-green-600', 'hover:bg-green-700');
                } else {
                    this.trackingToggle.classList.remove('bg-green-600', 'hover:bg-green-700');
                    this.trackingToggle.classList.add('bg-red-600', 'hover:bg-red-700');
                }
                
                this.updateStatus(statusMessage);
            }
            
            async handleGoTo() {
                const ra = this.raInput.value.trim();
                const dec = this.decInput.value.trim();
                
                if (!ra || !dec) {
                    this.updateStatus('Error: Please enter both RA and DEC coordinates.');
                    return;
                }
                
                this.goToButton.textContent = "Moving...";
                this.goToButton.disabled = true;

                const statusMessage = await this.controller.goTo(ra, dec);
                
                this.goToButton.textContent = "INITIATE GOTO";
                this.goToButton.disabled = false;

                this.updateStatus(statusMessage);
            }

            // --- UTILITY ---
            
            updateStatus(message) {
                this.statusMessage.textContent = message;
            }
        }

        // --- APPLICATION INITIALIZATION ---

        window.onload = function() {
            // Initialize the controller and UI after the DOM is ready
            const trackerController = new StarTrackerController();
            const uiHandler = new UIHandler(trackerController);

            uiHandler.updateStatus(`Controller initialized. Target IP: ${ESP32_IP}`);
        };
    </script>
</body>
</html>
)rawliteral";
