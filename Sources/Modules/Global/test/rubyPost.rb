#!/usr/bin/ruby

require 'cgi'
cgi = CGI.new
puts "Hello " + cgi['name'] + "!"