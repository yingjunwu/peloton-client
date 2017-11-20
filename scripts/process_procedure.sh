if [ "$#" -ne 4 ]; then
  echo "Please input parameters."
  echo "Example: "$0" scale_factor operation_count update_ratio zipf_theta"
  echo "The table size will be scale_factor * 1000."
  exit 1;
fi
../driver_main -y procedure -k $1$ -o $2$ -u $3$ -z $4$
