if [ "$#" -ne 0 ]; then
  echo "No parameter is required."
  echo "Example: "$0
  exit 1;
fi
../driver_main -y scan
