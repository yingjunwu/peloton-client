if [ "$#" -ne 2 ]; then
  echo "Please input parameters. Example: "$0" with_index scale_factor"
  echo "The table size will be scale_factor * 1000."
fi
../peloton_client -y populate $1$ $2$
