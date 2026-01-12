var hideResult;
var showAll;

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

$(document).ready(function() {

    let totalScenarios = data.passedScenarios  
        + data.failedScenarios
        + data.skippedScenarios;

    data.totalScenarios = totalScenarios;
    let totalDuration = 0;

    data.passedScenariosPercent = (data.passedScenarios * 100 / totalScenarios).toFixed(2);
    data.failedScenariosPercent = (data.failedScenarios * 100 / totalScenarios).toFixed(2);
    data.skippedScenariosPercent = (data.skippedScenarios * 100 / totalScenarios).toFixed(2);

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

    data.featureScenarios.forEach((scenario, scenarioIndex) => {
        scenario.passedSteps = 0;
        scenario.failedSteps = 0;
        scenario.undefinedSteps = 0;
        scenario.ambiguousSteps = 0;
        scenario.skippedSteps = 0;

		scenario.scenarioSteps.forEach((step, stepIndex) => {
            if (step.status == 1) {
                step.statusText = "passed";
                scenario.passedSteps++;
            } 
            else if (step.status == 2) {
                step.statusText = "failed";
                scenario.failedSteps++;
            } 
            else if (step.status == 3) {
                step.statusText = "undefined";
                scenario.undefinedSteps++;
            } 
            else if (step.status == 4) {
                step.statusText = "ambiguous";
                scenario.ambiguousSteps++;
            } 
            else if (step.status == 5) {
                step.statusText = "skipped";
                scenario.skippedSteps++;
            } 

            step.id = scenarioIndex + "-" + stepIndex;
            step.formattedDuration = formatDuration(step.duration);
        });
        
        let scenarioDuration = scenario.endTime - scenario.startTime;
        let scenarioStartTime = new Date(scenario.startTime);
        scenario.formattedDuration = formatDuration(scenarioDuration);
        scenario.startTime = scenarioStartTime.toLocaleDateString() + " " + scenarioStartTime.toLocaleTimeString();
        totalDuration += scenarioDuration;
	});

    data.formattedTotalDuration = formatDuration(totalDuration);

    document.title = "OPUS BDD Test Report - " + data.featureName;

    $('template').each(function() {
        let html = Mustache.render($(this).html(), data);
        $(this).replaceWith(html);
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
                "Pending",
                "Skipped",
                "Ambiguous",
                "Not defined"
            ],
            datasets: [{
                data: [
                    data.passedScenarios,
                    data.failedScenarios,
                    0,
                    data.skippedScenarios,
                    0,
                    0
                ],
                backgroundColor: [
                    "#26B99A",
                    "#E74C3C",
                    "#FFD119",
                    "#3498DB",
                    "#b73122",
                    "#F39C12"
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
    });

    

});
	
