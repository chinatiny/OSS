# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

import datetime
import logging

from socorro.lib import BadArgumentError, external_common
from socorro.external.postgresql.base import PostgreSQLBase
from socorro.external.postgresql.util import Util


logger = logging.getLogger("webapi")

report_type_sql = {
    'uptime': {
        "first_col": 'uptime_string',
        "first_col_format": 'category',
        "extra_join": ''' JOIN uptime_levels ON
            reports_clean.uptime >= min_uptime AND
            reports_clean.uptime < max_uptime ''',
    },

    'os': {
        'first_col': 'os_version_string',
        'first_col_format': 'category',
        'extra_join': ' JOIN os_versions USING ( os_version_id ) ',
    },

    'process_type': {
        'first_col': 'process_type',
        'first_col_format': 'category',
    },

    'architecture': {
        'first_col': 'architecture',
        'first_col_format': 'category',
    },

    'flash_version': {
        'first_col': 'flash_version',
        'first_col_format': '''CASE WHEN category = ''
            THEN 'Unknown/No Flash' ELSE category END''',
        'extra_join': ''' LEFT OUTER JOIN flash_versions
            USING (flash_version_id) ''',
    },
}


report_type_columns = {
    'uptime': 'uptime_string',
    'os': 'os_version_string',
    'process_type': 'process_type',
    'architecture': 'architecture',
    'flash_version': 'flash_version'
}


class SignatureSummary(PostgreSQLBase):

    def get(self, **kwargs):
        filters = [
            ("report_types", None, ["list", "str"]),
            ("report_type", None, "str"),
            ("signature", None, "str"),
            ("start_date", None, "date"),
            ("end_date", None, "date"),
            ("versions", None, ["list", "str"]),
        ]

        params = external_common.parse_arguments(filters, kwargs)
        if not params.get('report_types') and params.get('report_type'):
            # to support the legacy case
            individual_report = True
            report_types = [params['report_type']]
        else:
            individual_report = False
            report_types = params['report_types']

        # check that all the report types are recognized
        for report_type in report_types:
            query_params = report_type_sql.get(report_type, {})
            known_report_types = (
                'products',
                'distinct_install',
                'exploitability',
                'devices',
                'graphics'
            )
            if (
                report_type not in known_report_types and
                'first_col' not in query_params
            ):
                raise BadArgumentError(report_type)

        products = []
        versions = []

        # Get information about the versions
        util_service = Util(config=self.context)
        versions_info = util_service.versions_info(**params)
        if versions_info:
            for i, elem in enumerate(versions_info):
                products.append(versions_info[elem]["product_name"])
                versions.append(str(versions_info[elem]["version_string"]))

        # This MUST be a tuple otherwise it gets cast to an array
        params['product'] = tuple(products)
        params['version'] = tuple(versions)

        all_results = {}
        assert isinstance(params['start_date'], datetime.date)
        assert isinstance(params['end_date'], datetime.date)

        with self.get_connection() as connection:
            for report_type in report_types:
                result_cols, query_string, query_parameters = self._get_query(
                    report_type,
                    params
                )
                sql_results = self.query(
                    query_string,
                    params=query_parameters,
                    connection=connection
                )
                results = [dict(zip(result_cols, row)) for row in sql_results]
                all_results[report_type] = results

            if individual_report:
                return all_results.values()[0]
            else:
                return {'reports': all_results}

    def _get_query(self, report_type, params):

        if params['product'] and report_type is not 'products':
            product_list = ' AND product_name IN %s '
        else:
            product_list = ''

        if params['version'] and report_type is not 'products':
            version_list = ' AND version_string IN %s '
        else:
            version_list = ''

        if report_type == 'products':
            result_cols = ['product_name',
                           'version_string',
                           'report_count',
                           'percentage']
            query_string = """
            WITH crashes as (
                SELECT
                    product_name as category
                    , version_string
                    , SUM(report_count) as report_count
                FROM signature_summary_products
                    JOIN signatures USING (signature_id)
                WHERE signatures.signature = %s
                    AND report_date >= %s
                    AND report_date < %s
                GROUP BY product_name, version_string
            ),
            totals as (
                SELECT
                    category
                    , version_string
                    , report_count
                    , SUM(report_count) OVER () as total_count
                FROM crashes
            )
            SELECT category
                , version_string
                , report_count
                , round((report_count * 100::numeric)/total_count,3)::TEXT
                as percentage
            FROM totals
            ORDER BY report_count DESC"""
            query_parameters = (
                params['signature'],
                params['start_date'],
                params['end_date']
            )

        elif report_type == 'distinct_install':
            result_cols = ['product_name',
                           'version_string',
                           'crashes',
                           'installations']
            query_string = """
                SELECT product_name
                    , version_string
                    , SUM(crash_count) AS crashes
                    , SUM(install_count) AS installations
                FROM signature_summary_installations
                    JOIN signatures USING (signature_id)
                WHERE
                    signatures.signature = %s
                    AND report_date >= %s
                    AND report_date < %s
            """
            query_string += product_list
            query_string += version_list
            query_string += """
                GROUP BY product_name, version_string
                ORDER BY crashes DESC
            """
            query_parameters = (
                params['signature'],
                params['start_date'],
                params['end_date']
            )

            if product_list:
                query_parameters += (params['product'],)
            if version_list:
                query_parameters += (params['version'],)

        elif report_type == 'exploitability':
            # Note, even if params['product'] is something we can't use
            # that in this query
            result_cols = [
                'report_date',
                'null_count',
                'none_count',
                'low_count',
                'medium_count',
                'high_count',
            ]
            query_string = """
                SELECT
                    cast(report_date as TEXT),
                    SUM(null_count),
                    SUM(none_count),
                    SUM(low_count),
                    SUM(medium_count),
                    SUM(high_count)
                FROM exploitability_reports
                    JOIN signatures USING (signature_id)
                WHERE
                    signatures.signature = %s
                    AND report_date >= %s
                    AND report_date < %s
            """
            query_string += product_list
            query_string += version_list
            query_string += """
                GROUP BY report_date
                ORDER BY report_date DESC
            """
            query_parameters = (
                params['signature'],
                params['start_date'],
                params['end_date'],
            )

            if product_list:
                query_parameters += (params['product'],)
            if version_list:
                query_parameters += (params['version'],)

        elif report_type == 'devices':
            result_cols = [
                'cpu_abi',
                'manufacturer',
                'model',
                'version',
                'report_count',
                'percentage',
            ]
            query_string = """
                WITH crashes as (
                    SELECT
                        android_devices.android_cpu_abi as cpu_abi,
                        android_devices.android_manufacturer as manufacturer,
                        android_devices.android_model as model,
                        android_devices.android_version as version,
                        SUM(report_count) as report_count
                    FROM signature_summary_device
                        JOIN signatures USING (signature_id)
                        JOIN android_devices ON
                            signature_summary_device.android_device_id =
                            android_devices.android_device_id
                    WHERE signatures.signature = %s
                        AND report_date >= %s
                        AND report_date < %s
            """
            query_string += product_list
            query_string += version_list
            query_string += """
                    GROUP BY
                        android_devices.android_cpu_abi,
                        android_devices.android_manufacturer,
                        android_devices.android_model,
                        android_devices.android_version
                ),
                totals as (
                    SELECT
                        cpu_abi,
                        manufacturer,
                        model,
                        version,
                        report_count,
                        SUM(report_count) OVER () as total_count
                    FROM crashes
                )
                SELECT
                    cpu_abi,
                    manufacturer,
                    model,
                    version,
                    report_count,
                    round((report_count * 100::numeric)/total_count,3)::TEXT
                        as percentage
                FROM totals
                ORDER BY report_count DESC
            """
            query_parameters = (
                params['signature'],
                params['start_date'],
                params['end_date'],
            )

            if product_list:
                query_parameters += (params['product'],)
            if version_list:
                query_parameters += (params['version'],)

        elif report_type == 'graphics':
            result_cols = [
                'vendor_hex',
                'adapter_hex',
                'vendor_name',
                'adapter_name',
                'report_count',
                'percentage',
            ]
            query_string = """
                WITH crashes as (
                    SELECT
                        graphics_device.vendor_hex as vendor_hex,
                        graphics_device.adapter_hex as adapter_hex,
                        graphics_device.vendor_name as vendor_name,
                        graphics_device.adapter_name as adapter_name,
                        SUM(report_count) as report_count
                    FROM signature_summary_graphics
                        JOIN signatures USING (signature_id)
                        JOIN graphics_device ON
                            signature_summary_graphics.graphics_device_id =
                            graphics_device.graphics_device_id
                    WHERE signatures.signature = %s
                        AND report_date >= %s
                        AND report_date < %s
            """
            query_string += product_list
            query_string += version_list
            query_string += """
                    GROUP BY
                        graphics_device.graphics_device_id
                ),
                totals as (
                    SELECT
                        vendor_hex,
                        adapter_hex,
                        vendor_name,
                        adapter_name,
                        report_count,
                        SUM(report_count) OVER () as total_count
                    FROM crashes
                )
                SELECT
                    vendor_hex,
                    adapter_hex,
                    vendor_name,
                    adapter_name,
                    report_count,
                    round((report_count * 100::numeric)/total_count,3)::TEXT
                        as percentage
                FROM totals
                ORDER BY report_count DESC
            """
            query_parameters = (
                params['signature'],
                params['start_date'],
                params['end_date'],
            )

            if product_list:
                query_parameters += (params['product'],)
            if version_list:
                query_parameters += (params['version'],)

        elif report_type in report_type_columns:
            result_cols = ['category', 'report_count', 'percentage']
            query_string = """
                WITH crashes AS (
                    SELECT """
            query_string += report_type_columns[report_type]
            query_string += """ AS category
                        , sum(report_count) AS report_count
                    FROM signature_summary_"""
            query_string += report_type
            query_string += """
                        JOIN signatures USING (signature_id)
                    WHERE
                        signatures.signature = %s
                        AND report_date >= %s
                        AND report_date < %s
            """
            query_string += product_list
            query_string += version_list
            query_string += """
                    GROUP BY category
                ),
                totals AS (
                    SELECT
                        category
                        , report_count
                        , sum(report_count) OVER () as total_count
                    FROM crashes
                )
                SELECT category
                    , report_count
                    , round((report_count * 100::numeric)/total_count,3)::TEXT
                as percentage
                FROM totals
                ORDER BY report_count DESC
            """
            query_parameters = (
                params['signature'],
                params['start_date'],
                params['end_date']
            )

            if product_list:
                query_parameters += (params['product'],)
            if version_list:
                query_parameters += (params['version'],)

        return result_cols, query_string, query_parameters
