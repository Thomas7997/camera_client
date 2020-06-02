const express = require('express');
const router = express.Router();

const index = require('../controllers/index');

router.get('/capture', index.take_capture);

module.exports = router;