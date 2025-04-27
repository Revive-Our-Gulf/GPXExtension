# GPX Extension #

## What is it? ##

This is an extension of BlueOS designed for the operation of the BlueROV. The extension has the following functionality:

* It logs position and status information to a database.
* It takes video sequences as queries, and outputs corresponding [GPX](https://wiki.openstreetmap.org/wiki/GPX#:~:text=GPX%2C%20or%20GPS%20exchange%20format,and%20convert%20to%20other%20forms). files according to data that has been logged.

## Technical Details ##

* This project was spawned from the recommended BlueOS extension [template](https://github.com/BlueOS-community/blueos-extension-template).
* It is expected to run in a docker image, as is standard practice with BlueOS extensions. 
* Data is recorded to the [MariaDB](https://mariadb.org/) database, which is supposed to be lightweight (it runs on a Raspberry Pi 4).
The NGINX web server was replaced using [CrowCpp](https://crowcpp.org/master/) for web support (similar to Flask in Python). The main design choices here were speed and lightweight code.

## Project Setup ##

The project is made up of two main files:

* _Logger_: Reads information from the BlueROV and logs it to a database.
* _StatusTool: Provides the Extension GUI, contains the GPX generation logic and handles the queries.

### Repo installation
```
cd ~
mkdir Installs
cd Installs
git clone https://github.com/Revive-Our-Gulf/GPXExtension.git
```

## Starting Docker Sessions ##

### Mariadb ##

To install Mariadb for the BlueOS arm32/v7 RPI use:
```
docker run -d --restart unless-stopped --name mariadb -e MYSQL_ROOT_PASSWORD=808Rkief$ -p 3306:3306 yobasystems/alpine-mariadb
```
The docker container for the database can be launched as follows:

```docker run -d --restart unless-stopped --name mariadb -e MARIADB_ROOT_PASSWORD=808Rkief$ -p 3306:3306 -d mariadb:latest```

Note that a user needs to be created. So launch a console into the docker container

```docker exec -it mariadb mariadb -h 127.0.0.1 -u root -p808Rkief$```

and then add the user as follows:

```
CREATE USER 'trevor'@'%' IDENTIFIED BY '808Rkief$';
GRANT ALL ON *.* TO 'trevor'@'%';
FLUSH PRIVILEGES;
```

After exiting from the database by typing `exit`, the mariadb needs to be setup to have a BlueROV database with appropriate fields.
Copy the generate.sql file to the mariadb docker and enter the docker.
```
docker cp /home/pi/Installs/GPXExtension/sql/generate.sql mariadb:/var/lib/mysql/generate.sql
docker exec -it mariadb mariadb -h 127.0.0.1 -u root -p808Rkief$
```
Source the generate.sql file to create the BlueROV database and fields
```
SOURCE /var/lib/mysql/generate.sql;
```
After exiting back out with `exit` the IP address can be added as follows:
```
mariadb -h $(docker inspect -f '{{range.NetworkSettings.Networks}}{{.IPAddress}}{{end}}' mariadb) -u trevor -p808Rkief$ BlueROV
```

A docker network needs to be setup to allow the status tool to communicate with the database. This is done with:
```
sudo docker network create gpx_net
docker network connect --alias gpx_db gpx_net mariadb
```
The connect command also gives the mariadb database the gpx_db alias

### Logger (Plugin) ###
Now to start the Logger use:
```
cd /home/pi/Installs/GPXExtension/app/Logger/
sudo ./build.sh
```
This process will take a long time (up to 30 or so minutes the first time)

### Status Tool (Plugin) ###
To start the Status tool:
```
cd /home/pi/Installs/GPXExtension/app/StatusTool/
sudo ./build.sh
```

The Status Tool can be access from [192.168.2.2:5428](http://192.168.2.2:5428)
