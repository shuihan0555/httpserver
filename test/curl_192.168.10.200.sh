while(true)
do
	curl -H "Content-Type: application/json" -X POST -d '{"symbols":["EUR_USD","CHF_HKD","CHF_HKD","CHF_RMB", "EUR_RMB"]}' http://192.168.10.200:5001/md
	\n\r
done


