FROM ubuntu:latest
LABEL maintainer='sudarvendan@gmail.com'

COPY etc_asterisk /etc/asterisk
COPY usr_sbin /usr/sbin/
COPY var_log_asterisk /var/log/asterisk
COPY var_spool_asterisk /var/spool/asterisk
COPY usr_lib_asterisk /usr/lib/asterisk
COPY var_lib_asterisk /var/lib/asterisk
COPY var_run_asterisk /var/run/asterisk

RUN sed -i -e 's/# MAXFILES=/MAXFILES=/' /usr/sbin/safe_asterisk \
&& useradd -m asterisk -s /sbin/nologin \
&& chown -R asterisk:asterisk /var/run/asterisk \
                                  /etc/asterisk/ \
                                  /var/lib/asterisk \
                                  /var/log/asterisk \
                                  /var/spool/asterisk \
                                  /usr/sbin/asterisk \

