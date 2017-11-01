rsync -arv src/ yingjunw@cmu:~/peloton-client/src/
ssh yingjunw@cmu "rsync -arv ~/peloton-client/src/ dev2.db.pdl.cmu.local:~/peloton-client/src/"
rsync -arv include/ yingjunw@cmu:~/peloton-client/include/
ssh yingjunw@cmu "rsync -arv ~/peloton-client/include/ dev2.db.pdl.cmu.local:~/peloton-client/include/"
rsync -arv scripts/ yingjunw@cmu:~/peloton-client/scripts/
ssh yingjunw@cmu "rsync -arv ~/peloton-client/scripts/ dev2.db.pdl.cmu.local:~/peloton-client/scripts/"
scp Makefile yingjunw@cmu:~/peloton-client/Makefile
ssh yingjunw@cmu "scp ~/peloton-client/Makefile dev2.db.pdl.cmu.local:~/peloton-client/"
