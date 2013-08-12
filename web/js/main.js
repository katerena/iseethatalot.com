(function() {
    var ROOT_URL = '/';
    var VOTE_URL = 'vote';
    var ALOT_URL = 'alot';

    var ALOT_REFRESH_INTERVAL = 2500;
    var PLACEHOLDER_SELECTOR = '#thisAlot .generating';
    var ALOT_SELECTOR = '#thisAlot .alot';
    var ALOT_ID = null;

    function scheduleRefresh() {
        setTimeout(checkOnAlot, ALOT_REFRESH_INTERVAL);
    }

    function checkOnAlot() {
        console.log("Checking on alot...");

        $.get(ALOT_URL + "/" + ALOT_ID)
            .done(function(content) {
                $(ALOT_SELECTOR).replaceWith(content);
                var generatingMsg = $(PLACEHOLDER_SELECTOR);
                if (generatingMsg.length) {
                    scheduleRefresh();
                }
            })
            .fail(function() {
                alert("Sorry, we're having alot of problems :(");
                scheduleRefresh();
            });
    }

    function initAlotGeneration() {
        var generatingMsg = $(PLACEHOLDER_SELECTOR);

        if (generatingMsg.length) {
            ALOT_ID = generatingMsg.data('alot-id');
            scheduleRefresh();
        }
    }

    function initVoting() {
        //Get rid of any rating buttons for alots we already rated
        $('.alot-list-item').each(function() {
            var alot = $(this);
            var alotId = alot.data('alot-id');

            if (localStorage && localStorage.getItem) {
                var vote = localStorage.getItem(alotId);
                if (vote !== null) {
                    alot.find('.rating-buttons').remove();
                    alot.find('.rating-stats').removeClass('hide');
                }
            }
        });

        //Add click handlers for any rating buttons remaining
        $('.rating-buttons button').on('click', function() {
            var btn = $(this);
            var vote = btn.val();
            var alot = btn.parents('.alot-list-item');
            var alotId = alot.data('alot-id');

            //Disable the buttons
            var ratingButtons = btn.parent();
            ratingButtons.find('button').prop('disabled', true);

            //Send the vote
            $.post(VOTE_URL, {
                id: alotId,
                vote: vote
            })
                .done(function(voteStats) {
                    //Update the rating stats
                    alot.find('.rating-stats')
                        .html(voteStats)
                        .removeClass('hide');

                    //Remove the buttons
                    ratingButtons.remove();

                    //Remember that we've voted on this alot
                    if (localStorage && localStorage.setItem) {
                        localStorage.setItem(alotId, vote);
                    }
                })
                .fail(function() {
                    //Re-enable the buttons
                    ratingButtons.find('button').prop('disabled', false);
                });
        });
    }

    function initCreationForm() {
        var imageInput = $('.image-input');
        var wordInput = $('.word-input');

        $('form').on('submit', function(e) {
            var imageUrl = $.trim(imageInput.val());
            var word = $.trim(wordInput.val());

            if (!imageUrl) {
                imageInput.focus();
            }

            if (!word) {
                wordInput.focus();
            }

            if (!imageUrl || !word) {
                e.preventDefault();
                return false;
            }
        });
    }

    window.running_alot = function(options) {
        ROOT_URL = options.root || '/';
        VOTE_URL = ROOT_URL + VOTE_URL;
        ALOT_URL = ROOT_URL + ALOT_URL;

        initAlotGeneration();
        initCreationForm();
        initVoting();
    };
})();