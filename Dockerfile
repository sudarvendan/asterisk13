FROM ubuntu:latest
LABEL maintainer='sudarvendan@gmail.com'
ENV NUM 4

RUN apt-get update \
	&& apt-get install -y \
		build-essential \
		git-core \
		subversion \
		wget \
		libjansson-dev \
		sqlite \
		autoconf \
		automake \
		libxml2-dev \
		libncurses5-dev \
		libtool \
		uuid-dev \
		openssl \
		pkg-config \
		sqlite3 \
		libsqlite3-dev \
	&& cd /usr/src \
	&& git clone https://github.com/sudarvendan/asterisk13.git \
	&& cd asterisk13 \
	&& ./configure \
		--libexecdir=/usr/lib64 \
		--with-jansson \
	&& make clean \
	&& make \
	&& make install \
	&& make samples \
	&& make config \
	&& sed -i -e 's/# MAXFILES=/MAXFILES=/' /usr/sbin/safe_asterisk \
	&& useradd -m asterisk -s /sbin/nologin \
	&& chown -R asterisk:asterisk /var/run/asterisk \
                                  /etc/asterisk/ \
                                  /var/lib/asterisk \
                                  /var/log/asterisk \
                                  /var/spool/asterisk \
                                  /usr/lib64/asterisk \

USER asterisk
CMD /usr/sbin/asterisk -f
