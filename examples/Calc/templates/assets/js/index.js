function formatDuration(duration)
{
    let durationInMSec = duration;
    let hour = Math.floor(durationInMSec / 3600000);
    let min = Math.floor(durationInMSec % 3600000 / 60000);
    let sec = Math.floor(durationInMSec % 60000 / 1000);
    let msec = Math.floor(durationInMSec % 1000);
    let msecpad = "0".repeat(3 - String(msec).length) + msec;
    return [hour, min, sec].join(':') + '.' + msecpad;
}

function percent(n, total)
{
    return (n * 100 / total).toFixed(2);
}

$(document).ready(function () {
    data.totalScenarios = 0;
    data.totalPassedScenarios = 0;
    data.totalFailedScenarios = 0;
    data.totalSkippedScenarios = 0;

    data.features = data.features.filter((feature) => {
        return feature.skipped + feature.passed + feature.failed;
    });
    
    data.totalFeatures = data.passed + data.failed + data.skipped;

    data.features.forEach((feature) => {
        feature.featureFileName = feature.uri.split('/').pop();

        data.totalSkippedScenarios += feature.skipped;
        data.totalPassedScenarios += feature.passed;
        data.totalFailedScenarios += feature.failed;

        feature.totalScenarios = feature.passed + feature.failed + feature.skipped;

        data.totalScenarios += feature.totalScenarios;
        
        feature.formattedDuration = formatDuration(feature.duration);

        if (feature.status == "passed") {
            feature.statusIcon = "check-circle";
        } else if (feature.status == "failed") {
            feature.statusIcon = "exclamation-circle";
        } else if (feature.status == "skipped") {
            feature.statusIcon = "arrow-circle-right";
        }
    });

    data.featurePassedPercent = percent(data.passed, data.totalFeatures);
    data.featureFailedPercent = percent(data.failed, data.totalFeatures);
    data.featureSkippedPercent = percent(data.skipped, data.totalFeatures);

    data.passedScenariosPercent = percent(data.totalPassedScenarios, data.totalScenarios);
    data.failedScenariosPercent = percent(data.totalFailedScenarios, data.totalScenarios);
    data.skippedScenariosPercent = percent(data.totalSkippedScenarios, data.totalScenarios);

    const startDate = new Date(data.start_time);
    const endDate = new Date(data.end_time);
    data.executionStartTime = startDate.toLocaleDateString() + " " + startDate.toLocaleTimeString();
    data.executionEndTime = endDate.toLocaleDateString() + " " + endDate.toLocaleTimeString();
    data.executionRunTimeInHours = (data.duration / 3600000).toFixed(2);
    data.executionRunTimeInMins = (data.duration / 60000).toFixed(2);
    data.executionRunTimeInSeconds = (data.duration / 1000).toFixed(2);

    data.copyrightYear = new Date().getFullYear();
    data.copyrightOrganization = "XXX LLC.";
    
    $('template').each(function() {
        let html = Mustache.render($(this).html(), data);
        $(this).replaceWith(html);
    });

    $('#features-table').dataTable({
        "order": [[0, "asc"]],
        "lengthMenu": [[25, 50, 100, -1], [25, 50, 100, "All"]],
        "stateSave": true
    });

    var featureOptions = {
        legend: false,
        responsive: false
    };

    new Chart(document.getElementById("feature-chart"), {
        type: 'doughnut',
        tooltipFillColor: "rgba(51, 51, 51, 0.55)",
        data: {
            labels: [
                "Passed",
                "Failed",
                "Ambiguous",
                "Not Defined",
                "Pending",
                "Skipped"
            ],
            datasets: [{
                data: [
                    data.passed,
                    data.failed,
                    0,
                    0,
                    0,
                    data.skipped
                ],
                backgroundColor: [
                    "#26B99A",
                    "#E74C3C",
                    "#b73122",
                    "#F39C12",
                    "#FFD119",
                    "#3498DB"
                ]
            }]
        },
        options: featureOptions
    });

    var scenarioOptions = {
        legend: false,
        responsive: false
    };

    new Chart(document.getElementById("scenario-chart"), {
        type: 'doughnut',
        tooltipFillColor: "rgba(51, 51, 51, 0.55)",
        data: {
            labels: [
                "Passed",
                "Failed",
                "Ambiguous",
                "Not Defined",
                "Pending",
                "Skipped"
            ],
            datasets: [{
                data: [
                    data.totalPassedScenarios,
                    data.totalFailedScenarios,
                    0,
                    0,
                    0,
                    data.totalSkippedScenarios
                ],
                backgroundColor: [
                    "#26B99A",
                    "#E74C3C",
                    "#b73122",
                    "#F39C12",
                    "#FFD119",
                    "#3498DB"
                ]
            }]
        },
        options: scenarioOptions
    });

    $('.x_title').on('click', function() {
        var $BOX_PANEL = $(this).closest('.x_panel'),
            $ICON = $(this).find('.collapse-link i'),
            $BOX_CONTENT = $BOX_PANEL.find('.x_content');
    
        // fix for some div with hardcoded fix class
        if ($BOX_PANEL.attr('style')) {
            $BOX_CONTENT.slideToggle(200, function() {
                $BOX_PANEL.removeAttr('style');
            });
        } else {
            $BOX_CONTENT.slideToggle(200);
            $BOX_PANEL.css('height', 'auto');
        }
    
        $ICON.toggleClass('fa-chevron-up fa-chevron-down');
    });
    
    $('body').tooltip({
        selector: '[data-toggle="tooltip"]'
    });
    
    hideResult = (resultId) => {
        $('span[class*=step]').closest('div.x_panel[style]').hide();
        $('span[class*=' + resultId + ']').closest('div.x_panel[style]').show();
    }
    
    showAll = () => {
        $('span[class*=step]').closest('div.x_panel[style]').show();
    }
    
    const status = ['passed', 'failed', 'pending', 'skipped', 'ambiguous', 'not-defined']
    status.forEach(value => {
        var menuItem = $('span[class*=' + value + '-background]');
        if (menuItem.length === 0) {
            $('#' + value).parent().addClass('disabled');
        }
    })

});
