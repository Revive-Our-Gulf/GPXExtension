FROM alpine:3.18.3

# MariaDB server and client
RUN apk update && apk add mariadb mariadb-client

# this is not executed by mariadb package
RUN mysql_install_db --user=trevor

# make life easier
ENV TERM xterm

# RUN mkdir -p /site
# COPY files/register_service /site/register_service
# COPY site/ /site/

# Copy everything for your application
# COPY files/entrypoint.sh /entrypoint.sh

# Add docker configuration
LABEL permissions='{\
  "ExposedPorts": {\
    "80/tcp": {}\
  },\
  "HostConfig": {\
    "PortBindings": {\
      "80/tcp": [\
        {\
          "HostPort": ""\
        }\
      ]\
    }\
  }\
}'
LABEL authors='[\
    {\
        "name": "Peter Miles",\
        "email": "peter.miles@reviveourgulf.org.nz"\
    }\
]'
LABEL company='{\
    "about": "Restoring the health of te taiao and giving our wai a fighting chance to ensure it is stable enough to support us now and into the future",\
    "name": "Revive Our Gulf",\
    "email": "info@reviveourgulf.org.nz"\
}'
LABEL readme="https://github.com/Revive-Our-Gulf/GPXExtension/blob/main/README.md"
LABEL type="GPX"
LABEL tags='[\
  "GPX"\
]'

# ENTRYPOINT ["/entrypoint.sh"]
