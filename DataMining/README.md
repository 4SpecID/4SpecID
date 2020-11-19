# USAGE
1. Keep the three scripts under the same folder
2. From the Linux commandline run:
```
./DownloadFromBold.sh "NAME OF YOUR TAXA" OUTPUTFOLDER
```

If you want to donwload more than one taxonomic group at the sam time use "|" between keywords. For example, to download *Bovinae* and *Caprinae* and save the file at the current directory run:
```
./DownloadFromBold.sh "Bovinae|Caprinae" .
```

# Notes on Operating System
* These scripts are prepared to run on a Linux terminal.
* They should work on MacOS terminal as well but they were never tested.
* If you have a Windows PC you can install Windows Subsystem for Linux (the scripts were tested and work on the UBUNTU terminal of Windows Subsystem for Linux)


