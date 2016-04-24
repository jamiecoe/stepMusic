# stepMusic
Workshops in Creative Coding Project 1

The goal of the project was to explore ideas regarding ‘interactive music’, and play with how these ideas can affect the compositional process.

I was inspired by a number of different sources for this project:

Scott Snibbe: Interactive artist who has created interactive music applications such as Bubble Harp, which allows the user to create music by drawing animated patterns with their fingers
Brian Eno/Generative Music: Music created by a system that allows for the continuous and dynamic variation of a composition. I wanted to try bringing the user and computer together in a coupled system, where the resulting  interaction generates the composition.

The piece has a minimalist aesthetic, whereby a single instrument plays randomly selected notes from a dynamic minor pentatonic scale, gradually moving through the Circle Of Fifths progression. In this way the scale only changes by one note with each modulation, allowing for a subtle transformation of the harmony. Upon leaving the 12th scale, the piece returns to its original position, creating an infinite circular structure.

The overall tempo, the rhythmic patterns of the melody and the intensity of the sound itself are developed as a result of the coupled system of user and computer. This is achieved by tracking the number of steps taken and the walking speed of an individual user, information which is gathered using an iPhone accelerometer placed in the pocket of the user.

The technical implementation of the piece can be broken down into four sections:

The movement tracking uses step / speed detection, via data from an iPhone accelerometer. I used an app called GyrOSC, created by BitShape Software (http://www.bitshapesoftware.com/instruments/gyrosc/), in order to access and transfer accelerometer data to openFrameworks in the form of OSC messages. In order to manage the incoming data, which was quite noisy, I used the lo-pass filter from the ofxMaxim library which, by scaling the data by a factor of 0.2, gave much smoother results. I then used ofMap to map the data to a more readable range of 0-100.

The musical notation involved creating a 2d-array of frequencies in order to hold 12 scales, each containing 5 notes. I used an additional array named currentScale as a variable to contain the latest scale being used as they changed throughout the piece. Every 5 steps taken, a random note frequency is picked from currentScale and sent to the oscillator along with a trigger message to be played, as well as to the visualizer to move the colored circle to the correct note. Every 50 steps, currentScale moves to the next scale in the Circle of Fifths.

The sound design section firstly involves the setting up of a number of objects from the ofxMaxim library; an oscillator, an envelope, a filter and a delay object. In the audioOutput method I set the oscillator to play a saw wave, chosen in order to hear the maximum effect of the lo-pass filter. To trigger individual notes I used an envelope, which I set with a long release and put through an extended delay so that the individual notes could be blended against each other and thus create the evolving harmony of the piece. Finally the sound was put through a low pass filter, which gradually open and closed as the user sped up and slowed down. In this way the higher frequencies of the saw wave were gradually freed in order to increase the intensity of the sound as the intensity of the user’s movements increased.

The visualizer section was developed in order to visualize the musical processes at play in the piece. This consisted of a circular shape created by 13 white points, each representing a note from all the possible options of the scales. The coloured circle symbolises the note that had most recently been played. As a new note is triggered, the coloured circle travels across the screen to match its new position, leaving behind a trail created using a large vector of circles. These very gradually fade over time as the position of each point in the vector is made equal to the one after it, thus the trail seems to be very slowly trying to catch up to its latest point. With every change in scale, the colour of the circle similarly changes, moving across the colour spectrum in 1/12 increments so as to return to its original point as we return to the original scale. To mimic the opening and closing of the lo-pass filter, the saturation of the colour increases and decreases as the user speeds up and slows down. My overall intention was to map the patterns of the scales and sound intensity, visualizing the extended journey of the piece.

One challenge I face was using step detection in order to trigger notes in a way that produced musically interesting results. I started by triggering a note with every step but found this quickly led to an overload on the oscillator and problems with distortion. Instead, by triggering a note every 5 steps, enough space was allowed for new rhythmic and melodic patterns to continuously enter and dissipate.

In terms of future development for the project, it would be interesting to expand upon the use of other movement information. For example, I had access to a compass and gyroscope via the iPhone, but ultimately decided not to use them in order to retain the minimalist aesthetic. This data could however be used to broaden the interactive potential of the piece by controlling other parameters of the composition.


