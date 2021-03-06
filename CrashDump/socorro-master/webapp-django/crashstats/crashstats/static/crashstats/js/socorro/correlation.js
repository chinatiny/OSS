/* global window, $ */

window.correlations = (function () {
    /**
     * Handle any errors by logging them.
     */
    function handleError(error) {
        console.error(error.message);
    }

    var correlationData = {};

    function getDataURL(product) {
        if (product === 'Firefox') {
            return 'https://analysis-output.telemetry.mozilla.org/top-signatures-correlations/data/';
        } else if (product === 'FennecAndroid') {
            return 'https://analysis-output.telemetry.mozilla.org/top-fennec-signatures-correlations/data/';
        } else {
            return null;
        }
    }

    function loadChannelsData(product) {
        return Promise.resolve()
        .then(function () {
            if (correlationData[product]) {
                return correlationData[product];
            }

            var dataURL = getDataURL(product);
            if (!dataURL) {
                console.warn('Correlation results unavailable for the "' + product + '" product.');
                return null;
            }

            return fetch(dataURL + 'all.json.gz')
            .then(function (response) {
                return response.json();
            })
            .then(function (totals) {
                correlationData[product] = {
                    'date': totals['date'],
                };

                ['release', 'beta', 'aurora', 'nightly'].forEach(function (ch) {
                    correlationData[product][ch] = {
                        'total': totals[ch],
                        'signatures': {},
                    };
                });
            })
            .then(function () {
                return correlationData[product];
            });
        });
    }

    function loadCorrelationData(signature, channel, product) {
        return loadChannelsData(product)
        .then(function (channelsData) {
            if (!channelsData || signature in channelsData[channel]['signatures']) {
                return;
            }

            var shaObj = new jsSHA('SHA-1', 'TEXT');
            shaObj.update(signature);
            var sha1signature = shaObj.getHash('HEX');

            return fetch(getDataURL(product) + channel + '/' + sha1signature + '.json.gz')
            .then(function (response) {
                return response.json();
            })
            .then(function (data) {
                correlationData[product][channel]['signatures'][signature] = data;
            });
        })
        .catch(handleError)
        .then(function () {
            return correlationData;
        });
    }

    function itemToLabel(item) {
        return Object.getOwnPropertyNames(item)
        .map(function (key) {
            return key + ' = ' + item[key];
        })
        .join(' ∧ ');
    }

    // Convert the number to a user-readable percentage with four digits.
    // We use the same number of digits for any number (e.g. 100.0%, 03.45%) so
    // that the results are aligned, also when copied and pasted.
    function toPercentage(num) {
        var result = (num * 100).toFixed(2);

        if (result === '100.00') {
            return '100.0';
        }

        if (num < 0.1) {
            return '0' + result;
        }

        return result;
    }

    function confidenceInterval(count1, total1, count2, total2) {
        var prop1 = count1 / total1;
        var prop2 = count2 / total2;
        var diff = prop1 - prop2;

        // Wald 95% confidence interval for the difference between the proportions.
        var standard_error = Math.sqrt(prop1 * (1 - prop1) / total1 + prop2 * (1 - prop2) / total2);
        var ci = [diff - 1.96 * standard_error, diff + 1.96 * standard_error];

        // Yates continuity correction for the confidence interval.
        var correction = 0.5 * (1.0 / total1 + 1.0 / total2);

        return [ci[0] - correction, ci[1] + correction];
    }

    function sortCorrelationData(correlationData, total_reference, total_group) {
        return correlationData
        .sort(function (a, b) {
            // Sort by the number of attributes first (results with a smaller number of attributes
            // are easier to read and are often the most interesting ones).
            var rule_a_len = Object.keys(a.item).length;
            var rule_b_len = Object.keys(b.item).length;

            if (rule_a_len < rule_b_len) {
                return -1;
            }

            if (rule_a_len > rule_b_len) {
                return 1;
            }

            // Then, sort by percentage difference between signature and overall (using the lower endpoint
            // of the confidence interval of the difference).
            var ciA = confidenceInterval(a.count_group, total_group, a.count_reference, total_reference);
            var ciB = confidenceInterval(b.count_group, total_group, b.count_reference, total_reference);

            return Math.min(Math.abs(ciB[0]), Math.abs(ciB[1])) - Math.min(Math.abs(ciA[0]), Math.abs(ciA[1]));
        });
    }

    function getResults(signature, channel, product) {
        return loadCorrelationData(signature, channel, product)
        .then(function (data) {

            if (!data[product]) {
                return 'No correlation data was generated for the "' + product + '" product.';
            }

            if (!data[product][channel]['signatures'][signature] || !data[product][channel]['signatures'][signature]['results']) {
                return 'No correlation data was generated for the signature "' + signature + '" on the "' + channel + '" channel, for the "' + product + '" product.';
            }

            var correlationData = data[product][channel]['signatures'][signature]['results'];
            if (correlationData.length === 0) {
                return 'No correlations found for the signature "' + signature + '" on the "' + channel + '" channel, for the "' + product + '" product.';
            }

            var total_reference = data[product][channel].total;
            var total_group = data[product][channel]['signatures'][signature].total;

            var results = sortCorrelationData(correlationData, total_reference, total_group)
            .map(function (line) {
                var percentGroup = toPercentage(line.count_group / total_group);
                var percentRef = toPercentage(line.count_reference / total_reference);
                return '(' + percentGroup + '% in signature vs ' + percentRef + '% overall) ' + itemToLabel(line.item);
            });
            return results;
        })
        .catch(handleError);
    }

    return {
        getCorrelations: getResults,
    };
})();
