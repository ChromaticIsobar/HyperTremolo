/**
 * Initialize request from callback function
 * @param  {Function}       cbk Callback function
 * @return {XMLHttpRequest}     Request object
 */
function callback(cbk) {
  var request = new XMLHttpRequest();
  request.onreadystatechange = () => {
    if (request.readyState == 4 && request.status == 200) cbk(request);
  };
  return request;
}

/**
 * Make a callback function for appending to the release body
 * @param  {Element}  body     HTML Element to which to append the release body
 * @param  {Array}    releases Array of release
 * @param  {Number}   idx      Index of the first release to append (also append all next releases)
 * @return {Function}          Callback function
 */
function append_release_fn(body, releases, idx = 0) {
  return (request) => {
    var h2 = document.createElement("h2");
    var b = document.createElement("b");

    b.innerText = releases[idx].tag_name;
    h2.appendChild(b);
    body.appendChild(h2);

    var d = document.createElement("div");
    d.innerHTML = request.responseText;
    body.appendChild(d);

    append_release(body, releases, idx + 1);
  };
}

/**
 * Make requests for converting release body to HTML
 * @param  {Element} body     HTML Element to which to append the release body
 * @param  {Array}   releases Array of release
 * @param  {Number}  idx      Index of the first release to append (also append all next releases)
 */
function append_release(body, releases, idx = 0) {
  if (idx < 0 || idx >= releases.length) return;
  var request = callback(append_release_fn(body, releases, idx));
  request.open("POST", "https://api.github.com/markdown", true);
  request.setRequestHeader("Accept", "application/vnd.github.v3+json");
  request.send(
    JSON.stringify({
      text: releases[idx].body,
    })
  );
}

/**
 * Make callback function for releases request
 * @param  {Function} asset_name_fn Function for getting the desired asset name from the release object
 * @param  {Element}  body          HTML Element to which to append the release body
 * @param  {Element}  here          HTML anchor element to use for setting the download URL
 * @param  {Element}  here_showel   HTML Element to show after 'here_showtime' milliseconds
 * @param  {Number}   here_showtime Show the 'here_showel' element after this amount of milliseconds
 * @return {Function}               Callback function
 */
function request_releases_cbk(
  asset_name_fn,
  body,
  here,
  here_showel,
  here_showtime
) {
  return (request) => {
    var releases = JSON.parse(request.responseText);
    var asset_url = "";
    var asset_tag = "";
    var asset_release = "";

    // --- Get the latest release ---------------------------------------
    for (const release of releases) {
      for (const asset of release.assets) {
        if (asset.name == asset_name_fn(release)) {
          asset_url = asset.browser_download_url;
          asset_release = release;
          break;
        }
      }
      if (asset_url) break;
    }
    // ------------------------------------------------------------------

    // --- Set link and click it ----------------------------------------
    if (asset_url) {
      here.setAttribute("href", asset_url);
      here.setAttribute("download", asset_url);
      here.click();
      // Show clickable link after here_showtime milliseconds
      setTimeout(() => {
        here_showel.hidden = false;
      }, here_showtime);
    }
    // ------------------------------------------------------------------

    // --- Include changelog --------------------------------------------
    setTimeout(() => {
      append_release(body, releases);
    }, 10);
    // ------------------------------------------------------------------
  };
}

/**
 * Populate redirect page
 * @param  {String}   releases_endpoint Endpoint for the GitHub releases API
 * @param  {Function} asset_name_fn     Function for getting the desired asset name from the release object
 * @param  {Element}  body              HTML Element to which to append the release body
 * @param  {Element}  here              HTML anchor element to use for setting the download URL
 * @param  {Element}  here_showel       HTML Element to show after 'here_showtime' milliseconds
 * @param  {Number}   here_showtime     Show the 'here_showel' element after this amount of milliseconds
 */
function populate_page(
  releases_endpoint,
  asset_name_fn,
  body,
  here,
  here_showel = null,
  here_showtime = 5000
) {
  if (here_showel === null) here_showel = here;
  var request = callback(
    request_releases_cbk(asset_name_fn, body, here, here_showel, here_showtime)
  );
  request.open("GET", releases_endpoint, true);
  request.setRequestHeader("Accept", "application/vnd.github.v3+json");
  request.send(null);
}
