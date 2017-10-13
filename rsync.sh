rsync -arv src/ yingjunw@cmu:~/peloton-client/src/
ssh yingjunw@cmu "rsync -arv ~/peloton-client/src/ dev2.db.pdl.cmu.local:~/peloton-client/src/"
rsync -arv include/ yingjunw@cmu:~/peloton-client/include/
ssh yingjunw@cmu "rsync -arv ~/peloton-client/include/ dev2.db.pdl.cmu.local:~/peloton-client/include/"
