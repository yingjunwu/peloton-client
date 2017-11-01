if [ "$#" -ne 5 ]; then
  echo "Please input parameters. Example: "$0" with_prepared_statement scale_factor operation_count update_ratio zipf_theta"
  echo "The table size will be scale_factor * 1000."
  exit 1;
fi
../peloton_client -y client $1$ $2$ $3$ $4$ $5$
