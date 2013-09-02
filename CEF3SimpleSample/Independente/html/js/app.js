$(document).ready(function() {
    $('.btn').click(function() {
        window.cpp.ChangeTextInJS('Hello World!');
    });
});

function ChangeText(text) {
    $('#text').html(text);
}