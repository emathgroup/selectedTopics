import React from "react"
import UserLink from './userLinks'
import config from '../../data/siteConfig'

const Footer = () => {
    return(
        <div><UserLink link={config.userLinks}/></div>
    )
}
export default Footer