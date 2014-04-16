CC = gcc
CFLAGS = -Wall
TARGET = client
VPATH = src:include

default:
	cd database; make
	cd filelocks; make
	cd fifos; make
