# Running the Application
To run the application, run: `python app.py`

All functionality stuff is in app.py , everything scrolling plot related is in plotter.py

Still some functionality errors with pyqtgraph's Power Spectrum feature.

## Dependency

In order to launch the app, we need to install the `pyqt5`. (pyqtgraph included)

### Installing dependencies on an Ubuntu/Debian systems

Run the followings in the terminal:
`sudo apt-get install python pip python-qt5 (&& sudo -H pip install --upgrade pip pyqtgraph`)

### Installing dependencies on the macOS systems

Run the followings in order in the terminal:

Install Homebrew: `/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`

Install pyqt5 module: `brew install pyqt5`

(Not needed: Install pyqtgraph module using pip : `sudo -H pip install --upgrade pyqtgraph`)
