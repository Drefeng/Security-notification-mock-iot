const express = require('express');
const app = express();

/* CONFIGURATION */
const config = {
	projectId: '',
	keyFilename: './gcp_private_key.json'
}
const datastore = require('@google-cloud/datastore')(config);

//Routing
app.get('/data', (req, res) => {
res.status(200);    
res.setHeader('Content-Type', 'application/json');
const query = datastore.createQuery('ParticleEvent').order('timestamp', {descending: true});

datastore.runQuery(query, (err, entities, info) => {
    res.json(entities);
    res.end();
	});
});

//Routing
app.get('/latest', (req, res) => {
	res.status(200);    
	res.setHeader('Content-Type', 'application/json');
	const query = datastore.createQuery('ParticleEvent').order('timestamp', {descending: true}).limit(1);
	datastore.runQuery(query, (err, entities, info) => {
		res.json(entities);
		res.end();
		});
	});

app.listen(1234, () => console.log("listening on 1234!"));