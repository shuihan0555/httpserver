while(true)
do
	curl -H "Content-Type: application/json" -X POST -d '{"symbols":["EUR_USD","CHF_HKD","CHF_HKD","CHF_RMB", "EUR_RMB"]}' http://10.10.10.111:5002/md
	\n\r
done


