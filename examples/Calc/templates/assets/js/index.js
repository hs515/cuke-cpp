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
    data.passedFeatures = 0;
    data.failedFeatures = 0;
    data.skippedFeatures = 0;

    data.features = data.features.filter((feature) => {
        return feature.skippedScenarios + 
               feature.passedScenarios + 
               feature.failedScenarios;
    });
    
    data.features.forEach((feature) => {

        data.totalSkippedScenarios += feature.skippedScenarios;
        data.totalPassedScenarios += feature.passedScenarios;
        data.totalFailedScenarios += feature.failedScenarios;

        feature.totalScenarios = feature.passedScenarios 
            + feature.failedScenarios
            + feature.skippedScenarios;

        data.totalScenarios += feature.totalScenarios;
        
        feature.formattedDuration = formatDuration(feature.duration);

        if (feature.status == "passed" || feature.status == 1) {
            feature.statusText = "passed";
            feature.statusIcon = "check-circle";
            data.passedFeatures++;
        } else if (feature.status == "failed" || feature.status == 2) {
            feature.statusText = "failed";
            feature.statusIcon = "exclamation-circle";
            data.failedFeatures++;
        } else if (feature.status == "skipped" || feature.status == 5) {
            feature.statusText = "skipped";
            feature.statusIcon = "arrow-circle-right";
            data.skippedFeatures++;
        }
    });

    data.totalFeatures = data.passedFeatures 
        + data.failedFeatures
        + data.skippedFeatures;

    data.featurePassedPercent = percent(data.passedFeatures, data.totalFeatures);
    data.featureFailedPercent = percent(data.failedFeatures, data.totalFeatures);
    data.featureSkippedPercent = percent(data.skippedFeatures, data.totalFeatures);

    data.passedScenariosPercent = percent(data.totalPassedScenarios, data.totalScenarios);
    data.failedScenariosPercent = percent(data.totalFailedScenarios, data.totalScenarios);
    data.skippedScenariosPercent = percent(data.totalSkippedScenarios, data.totalScenarios);

    const duration = data.endTime - data.startTime;
    const startDate = new Date(data.startTime);
    const endDate = new Date(data.endTime);
    data.executionStartTime = startDate.toLocaleDateString() + " " + startDate.toLocaleTimeString();
    data.executionEndTime = endDate.toLocaleDateString() + " " + endDate.toLocaleTimeString();
    data.executionRunTimeInHours = (duration / 3600000).toFixed(2);
    data.executionRunTimeInMins = (duration / 60000).toFixed(2);
    data.executionRunTimeInSeconds = (duration / 1000).toFixed(2);

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
                    data.passedFeatures,
                    data.failedFeatures,
                    0,
                    0,
                    0,
                    data.skippedFeatures
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