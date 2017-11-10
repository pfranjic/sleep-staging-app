# sleep-staging-app (in progress)
A small Qt GUI application I made in Visual Studio in the process of studying C++ (and Visual Studio). The app predicts sleep stages for an individual polysomnographic recording, having analyzed a part of it.

## Input
The app lets the user load polysomnographic recordings in EDF format. Currently it supports importing of SC* files from the publicly available [Sleep-EDF Expanded](https://physionet.org/pn4/sleep-edfx/) Physionet database, alongside the associated annotations in .txt format. The app analyzes the Pz-Oz electroencephalographic (EEG) channel from the polysomnographic recording. The signal is segmented into 30-second epoch and each epoch has an associated annotation - one of the 6 stages of wakefulness/sleep according to the Rechtschaffen-Kales criteria (wakefulness, sleep stages 1, 2, 3, 4 and REM phase). The data is available at the [Physionet ATM](https://physionet.org/cgi-bin/atm/ATM)

## Signal processing & machine learning
The user is first prompted to select the range of epochs to be analyzed. Currently the features available for extraction are related to the relative power spectral density of the four major EEG frequency bands (alpha, beta, delta and theta), as estimated with the Fast Fourier Transform (FFT). The app lets the user decide between two classifiers, the Linear Discriminant Analysis and a Support Vector Machine with a Gaussian Radial Basis Function kernel. Prior to training the machine learning algorithm, the user has to specify the percentage of data to be used for estimating the performance of the classifier.

## Output
The application outputs the recall and precision for each of the 6 stages of wakefulness/sleep and overall accuracy. 

### Frameworks used
Qt framework

### Libraries used
Armadillo, Boost, FFTW3, Sigpack, Shark

### Screenshot
![sleep staging app image](https://image.ibb.co/j2x30m/ss.png)
